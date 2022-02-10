#ifndef SLE_EVENTACTION_H
#define SLE_EVENTACTION_H

#include "sle/dispatcher.h"
#include <memory>
#include <map>
#include <typeinfo>
#include <functional>


namespace sle
{

class EventActionBase
{
public:
    virtual ~EventActionBase() = default;
};

template <class EventType>
class EventAction:
    public EventActionBase,
    public EventHandler<EventType>
{
public:
    EventAction(
        const DispatcherPtr& dispatcher,
        std::function<void(const EventType&)>&& handler) :
    dispatcher_(dispatcher),
    handler_(handler)
    {}

    ~EventAction()
    {
        dispatcher_->unsubscribe(subId_);
    }

    void enable(
        const std::shared_ptr<EventAction<EventType>>& self)
    {
        subId_ = dispatcher_->subscribe<EventType>(self);
    }

    void handle(const EventType& e)
    {
        if (handler_)
            handler_(e);
    }

private:
    const DispatcherPtr& dispatcher_;
    std::function<void(const EventType&)> handler_;
    Dispatcher::SubscriptionId subId_;
};

class EventActionContainer
{
public:
    EventActionContainer(
        const DispatcherPtr& dispatcher) :
    dispatcher_(dispatcher)
    {}

    template <class EventType>
    void on(std::function<void(const EventType&)>&& handler)
    {
        auto action = std::make_shared<EventAction<EventType>>(
            dispatcher_,
            std::move(handler));
        action->enable(action);
        actions_[std::type_index(typeid(EventType))] = action;
    }

private:
    const DispatcherPtr& dispatcher_;
    std::map<std::type_index, std::shared_ptr<EventActionBase>> actions_;
};

} //namespace sle

#endif // SLE_EVENTACTION_H
