#include "sle/poll.h"
#include "sle/dispatcher.h"
#include <unistd.h>

namespace sle
{

PollFd::PollFd(
    const DispatcherPtr& dispatcher,
    int fd) :
dispatcher_(dispatcher),
fd_(fd),
proxy_(new Proxy(*this))
{}

PollFd::~PollFd()
{
    disarm();
}

void PollFd::armRead(
    std::function<void(PollFd&)> action)
{
    readAction_.swap(action);
    dispatcher_->addReadPoll(proxy_);
}

void PollFd::armWrite(
    std::function<void(PollFd&)> action)
{
    writeAction_.swap(action);
    dispatcher_->addWritePoll(proxy_);
}

void PollFd::suspendRead()
{
    dispatcher_->removeReadPoll(proxy_);
}

void PollFd::suspendWrite()
{
    dispatcher_->removeWritePoll(proxy_);
}

void PollFd::resumeRead()
{
    if (!readAction_)
    {
        // TODO: throw something
        return;
    }

    dispatcher_->addReadPoll(proxy_);
}

void PollFd::resumeWrite()
{
    if (!writeAction_)
    {
        // TODO: throw something
        return;
    }

    dispatcher_->addWritePoll(proxy_);
}

void PollFd::disarmRead()
{
    suspendRead();
    std::function<void(PollFd&)> dummy;
    readAction_.swap(dummy);
}

void PollFd::disarmWrite()
{
    suspendWrite();
    std::function<void(PollFd&)> dummy;
    writeAction_.swap(dummy);
}

void PollFd::readTrigger()
{
    if (readAction_)
        readAction_(*this);
}

void PollFd::writeTrigger()
{
    if (writeAction_)
        writeAction_(*this);
}

} // namespace sle
