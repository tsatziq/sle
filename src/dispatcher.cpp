#include "sle/dispatcher.h"
#include "sle/poll.h"
#include <forward_list>
#include <map>
#include <unistd.h>
#include <string.h>
#include <atomic>
#include <mutex>
#include <type_traits>
#include <iostream>

namespace sle
{

class DispatcherImpl;
using DispatcherImplPtr = std::shared_ptr<DispatcherImpl>;
using DispatcherImplWPtr = std::weak_ptr<DispatcherImpl>;    

class DispatcherImpl :
    public Dispatcher
{
public:
    DispatcherImpl();
    virtual ~DispatcherImpl();

    void dispatch() override;

    void stopDispatching() override;

    void addReadPoll(
        const PollFd::ProxyPtr& poller) override;

    void addWritePoll(
        const PollFd::ProxyPtr& poller) override;

    void removeReadPoll(
        const PollFd::ProxyPtr& poller) override;

    void removeWritePoll(
        const PollFd::ProxyPtr& poller) override;

    SubscriptionId subscribe(
        std::type_index type,
        std::shared_ptr<EventSubscriptionBase>&& subscription) override;

    void unsubscribe(
        SubscriptionId id) override;

    void queueEvent(
        std::unique_ptr<EventBase>&& event) override;

    SubscriptionList getSubscribers(
        std::type_index type) override;

private:
    void wakeSelect();

    using PollMap = std::map<int, PollFd::ProxyWPtr>;

    void addSelection(
        PollMap& toMap,
        const PollFd::ProxyPtr& poller);

    void removeSelection(
        PollMap& toMap,
        const PollFd::ProxyPtr& poller);

    void executeFdSet(
        fd_set* fdSet,
        const PollMap& activeMap,
        bool write);

    using SubIdToSubMap =
        std::map<SubscriptionId, std::shared_ptr<EventSubscriptionBase>>;

    using SubTypeMap =
        std::map<std::type_index, SubIdToSubMap>;

    using SubIdToSubTypeMap =
        std::map<SubscriptionId, std::type_index>;

    std::atomic_bool done_;
    std::mutex dataMutex_;
    PollMap readMap_;
    PollMap writeMap_;
    fd_set currentReadSelected_;
    fd_set currentWriteSelected_;
    int currentMaxFd_;
    int selectWakeFds_[2];
    
    SubTypeMap subscriptions_;
    SubIdToSubTypeMap subIds_;
    std::list<std::unique_ptr<EventBase>> events_;
    SubscriptionId nextId_ = 0; 
};

DispatcherPtr Dispatcher::createDefault()
{
    return DispatcherPtr(new DispatcherImpl);
}

DispatcherImpl::DispatcherImpl() :
    done_(false)
{
    pipe(selectWakeFds_);
    currentMaxFd_ = selectWakeFds_[0];
    FD_ZERO(&currentReadSelected_);
    FD_ZERO(&currentWriteSelected_);
    FD_SET(selectWakeFds_[0], &currentReadSelected_);
}

DispatcherImpl::~DispatcherImpl()
{
    close(selectWakeFds_[0]);
    close(selectWakeFds_[1]);
}

void DispatcherImpl::dispatch()
{
    fd_set rfds;
    fd_set wfds;

    while (!done_)
    {
        while(!events_.empty())
        {
			events_.front()->deliver();
			events_.pop_front();
        } 

		sendEvent(AllEventsHandled());

		while (!events_.empty())
		{
			events_.front()->deliver();
			events_.pop_front();
		}

		if (done_)
			break;

        memcpy(&rfds, &currentReadSelected_, sizeof(rfds));
        memcpy(&wfds, &currentReadSelected_, sizeof(wfds));

        int nReady = select(
            currentMaxFd_ + 1,
            &rfds,
            &wfds,
            nullptr,
            nullptr);

        if (nReady < 0 && errno != EBADF)
        {
            perror("select");
            return;
        }

        if (nReady == 0)
            continue;

        if (FD_ISSET(selectWakeFds_[0], &rfds))
        {
            char dummybuf[128];
            read(selectWakeFds_[0], dummybuf, sizeof(dummybuf));
        }

        executeFdSet(&rfds, readMap_, false);
        executeFdSet(&wfds, writeMap_, true);
    }
}

void DispatcherImpl::stopDispatching()
{
    done_ = true;
}

void DispatcherImpl::addReadPoll(
    const PollFd::ProxyPtr& poller)
{
    addSelection(readMap_, poller);
    FD_SET(poller->fd(), &currentReadSelected_);
}

void DispatcherImpl::addWritePoll(
    const PollFd::ProxyPtr& poller)
{
    addSelection(writeMap_, poller);
    FD_SET(poller->fd(), &currentWriteSelected_);
}

void DispatcherImpl::removeReadPoll(
    const PollFd::ProxyPtr& poller)
{
    FD_CLR(poller->fd(), &currentReadSelected_);
    removeSelection(readMap_, poller);
}

void DispatcherImpl::removeWritePoll(
    const PollFd::ProxyPtr& poller)
{
    FD_CLR(poller->fd(), &currentWriteSelected_);
    removeSelection(writeMap_, poller);
}

void DispatcherImpl::wakeSelect()
{
    write(selectWakeFds_[1], "x", 1);
}

void DispatcherImpl::executeFdSet(
    fd_set* fdSet,
    const PollMap& activeMap,
    bool write)
{
    std::forward_list<PollFd::ProxyWPtr> todoStack;
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        for (const auto& mapping : activeMap)
        {
            int fd = mapping.first;
            if (!FD_ISSET(fd, fdSet))
                continue;
            todoStack.emplace_front(mapping.second);
        }
    }

    for (const auto& pollerWeak : todoStack)
    {
        auto poller = pollerWeak.lock();
        if (write)
        {
            poller->writeTrigger();
        } else {
            poller->readTrigger();
        }

    }
}

void DispatcherImpl::addSelection(
    PollMap& toMap,
    const PollFd::ProxyPtr& poller)
{
    int fd = poller->fd();
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        toMap[fd] = poller;
    }

    if (fd > currentMaxFd_)
        currentMaxFd_ = fd;

    wakeSelect();
}

void DispatcherImpl::removeSelection(
    PollMap& fromMap,
    const PollFd::ProxyPtr& poller)
{
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        fromMap.erase(poller->fd());
    }

    wakeSelect();
}

Dispatcher::SubscriptionId DispatcherImpl::subscribe(
    std::type_index type,
    std::shared_ptr<EventSubscriptionBase>&& subscription)
{
    subscriptions_[type].emplace(nextId_, std::move(subscription));
    subIds_.emplace(nextId_, type);
    return nextId_++;
}

void DispatcherImpl::unsubscribe(
    SubscriptionId id)
{
    const auto typeIt = subIds_.find(id);

    if (typeIt == subIds_.end())
        return;

    auto& subMap = subscriptions_[typeIt->second];
    subMap.erase(id);
    subIds_.erase(typeIt);
}

void DispatcherImpl::queueEvent(
    std::unique_ptr<EventBase>&& event)
{
    events_.emplace_back(std::move(event));
    // dispatch(); // this should not be needed
}

Dispatcher::SubscriptionList DispatcherImpl::getSubscribers(
    std::type_index type)
{
    SubscriptionList ret;
    const auto& subs(subscriptions_[type]);

    for (const auto& idSubPair : subs)
        ret.push_back(idSubPair.second);

    return ret;
}

}
