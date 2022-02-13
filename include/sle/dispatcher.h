#ifndef SLE_DISPATCHER_H
#define SLE_DISPATCHER_H

#include "sle/poll.h"
#include "sle/event.h"
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <list>

namespace sle
{

class Dispatcher;
using DispatcherPtr = std::shared_ptr<Dispatcher>;
using DispatcherWPtr = std::weak_ptr<Dispatcher>;

class PollFd;

class Dispatcher
{
public:
    using SubscriptionId = unsigned long;

protected:
    using SubscriptionList =
        std::list<std::shared_ptr<EventSubscriptionBase>>;

    virtual SubscriptionId subscribe(
        std::type_index type,
        std::shared_ptr<EventSubscriptionBase>&& subscription) = 0;

    virtual void queueEvent(
        std::unique_ptr<EventBase>&& event) = 0;

public:
    static DispatcherPtr createDefault();

    virtual ~Dispatcher() = default;

    virtual void dispatch() = 0;

    virtual void stopDispatching() = 0;

    virtual void addReadPoll(
        const PollFd::ProxyPtr& poller) = 0;
    
    virtual void addWritePoll(
        const PollFd::ProxyPtr& poller) = 0;

    virtual void removeReadPoll(
        const PollFd::ProxyPtr& poller) = 0;

    virtual void removeWritePoll(
        const PollFd::ProxyPtr& poller) = 0;

    template<class EventDataType>
    SubscriptionId subscribe(
        const EventHandlerWPtr<EventDataType>& subscriber)
    {
        return subscribe(
            std::type_index(typeid(EventDataType)),
            std::shared_ptr<EventSubscriptionBase>(
                new EventSubscription<EventDataType>(
                    subscriber)));
    };

    virtual void unsubscribe(SubscriptionId id) = 0;

    template<class EventDataType>
    void sendEvent(
        const EventDataType& data)
    {
        const SubscriptionList subs =
            getSubscribers(std::type_index(typeid(EventDataType)));

        for (const auto& subBase : subs)
        {
            auto sub(
                std::dynamic_pointer_cast<
                    EventSubscription<EventDataType>>(subBase));

            if(sub) {
                auto eventPtr(sub->createEvent(data));
                if (eventPtr)
                    queueEvent(std::move(eventPtr));
            }
        }
    }

    template<class EventDataType>
    void sendEvent(
        EventHandlerPtr<EventDataType> receiver,
        const EventDataType& data)
    {
        queueEvent(std::unique_ptr<EventBase>(
            new Event<EventDataType>(receiver, data)));
    }

    class AllEventsHandled
    {};

protected:
    Dispatcher() = default;

    virtual SubscriptionList getSubscribers(std::type_index type) = 0;

private:
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;
};

} // namespace sle 

#endif
