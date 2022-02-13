#ifndef SLE_POLLFD_H
#define SLE_POLLFD_H

#include <functional>
#include <memory>

namespace sle
{

class Dispatcher;
using DispatcherPtr = std::shared_ptr<Dispatcher>;

class PollFd final
{
public:
    explicit PollFd(
        const DispatcherPtr& dispatcher,
        int fd);

    ~PollFd();

    void armRead(std::function<void(PollFd&)> action);
    void armWrite(std::function<void(PollFd&)> action);
    void suspendRead();
    void suspendWrite();
    void resumeRead();
    void resumeWrite();
    void disarmRead();
    void disarmWrite();

    void disarm()
    {
        disarmRead();
        disarmWrite();
    }

    int fd() const
    { return fd_; }

    void readTrigger();

    void writeTrigger();

    class Proxy
    {
    public:
        Proxy(
            PollFd& parent) :
        parent_(parent)
        {}

		void readTrigger()
		{ parent_.readTrigger(); }

		void writeTrigger()
		{ parent_.writeTrigger(); }

		int fd() const
        { return parent_.fd_; }

    private:
        PollFd& parent_;
    };

    using ProxyPtr = std::shared_ptr<Proxy>;
    using ProxyWPtr = std::weak_ptr<Proxy>;

private:
    DispatcherPtr dispatcher_;
    int fd_;
    ProxyPtr proxy_;
    std::function<void(PollFd&)> readAction_;
    std::function<void(PollFd&)> writeAction_;

    PollFd(const PollFd&) = delete;
    PollFd& operator=(const PollFd&) = delete;
};

} // namespace sle

#endif // SLE_POLLFD_H
