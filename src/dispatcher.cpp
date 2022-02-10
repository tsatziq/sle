#include "sle/dispatcher.h"
#include <forward_list>
#include <map>
#include <unistd.h>
#include <string.h>
#include <atomic>
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
    using SubIdToSubMap =
        std::map<SubscriptionId, std::shared_ptr<EventSubscriptionBase>>;

    using SubTypeMap =
        std::map<std::type_index, SubIdToSubMap>;

    using SubIdToSubTypeMap =
        std::map<SubscriptionId, std::type_index>;

    SubTypeMap subscriptions_;
    SubIdToSubTypeMap subIds_;
    std::list<std::unique_ptr<EventBase>> events_;
    SubscriptionId nextId_ = 0; 
    std::atomic_bool done_;
};

DispatcherPtr Dispatcher::createDefault()
{
    return DispatcherPtr(new DispatcherImpl);
}

DispatcherImpl::DispatcherImpl() :
    done_(false)
{}

DispatcherImpl::~DispatcherImpl()
{}

void DispatcherImpl::dispatch()
{
    while (!done_)
    {
        std::cout << "Start dispatch loop\n";
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
    }
}

void DispatcherImpl::stopDispatching()
{
    done_ = true;
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
    dispatch();
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
