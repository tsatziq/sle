#ifndef SLE_EVENT_H
#define SLE_EVENT_H

#include <memory>
#include <mutex>

namespace sle
{

template<class EventDataType>
class EventHandler
{
public:
    virtual ~EventHandler() = default;
    virtual void handle(const EventDataType& data) = 0;
};

template<class EventDataType>
using EventHandlerPtr = std::shared_ptr<EventHandler<EventDataType>>;

template<class EventDataType>
using EventHandlerWPtr = std::weak_ptr<EventHandler<EventDataType>>;

class EventBase
{
public:
    virtual ~EventBase() = default;
    virtual void deliver() = 0;

protected:
    EventBase() = default;

private:
    EventBase(const EventBase&) = delete;
    EventBase& operator=(const EventBase&) = delete;
};

template<class EventDataType>
class Event :
    public EventBase
{
public:
    Event(
        EventHandlerWPtr<EventDataType> subscriber,
        const EventDataType& data) :
    subscriber_(subscriber),
    data_(data)
    {}

    virtual ~Event() = default;

    void deliver() override
    {
        auto subscriber = subscriber_.lock();
        if (!subscriber)
            return;

        subscriber->handle(data_);
    }

private:
    EventHandlerWPtr<EventDataType> subscriber_;
    EventDataType data_;
};

class EventSubscriptionBase
{
public:
    virtual ~EventSubscriptionBase() = default;

protected:
    EventSubscriptionBase() = default;
};

template <class EventDataType>
class EventSubscription :
    public EventSubscriptionBase
{
public:
    EventSubscription(
            const EventHandlerWPtr<EventDataType>& subscriber) :
    subscriber_(subscriber)
    {}

    std::unique_ptr<EventBase> createEvent(
        const EventDataType& data)
    {
        return std::unique_ptr<EventBase>(
            new Event<EventDataType>(subscriber_, data));
    }

private:
    EventHandlerWPtr<EventDataType> subscriber_;
};

} // namespace sle
// tarviiko enaa eventsubscriber classia dispatcher.h:sta?

#endif
