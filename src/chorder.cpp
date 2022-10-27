#include "sle/chorder.h"
#include "sle/context.h"
#include "sle/types.h"
#include <memory>
#include <chrono>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace sle
{

class ChorderImpl : public Chorder
{
public:
    static std::unique_ptr<ChorderImpl> create(
        const Context* context)
    {
        return std::unique_ptr<ChorderImpl>(new ChorderImpl(context));
    }

    ~ChorderImpl();

    void tryChord(const char c) override;

private:
    ChorderImpl(const Context* context);

    void clearChord();

    class Timer;
    const Context* c_ = nullptr;
};

class ChorderImpl::Timer
{
public:
    Timer(size_t time, const std::function<void(void)>& f)
        : time_(std::chrono::milliseconds{time}), f_(f)
    {}

    ~Timer()
    {
        wait_thread.join();
    }

    void stop()
    {
        stop_ = true;
    }

private:
    void wait()
    {
        std::unique_lock<std::mutex> lock{mtx_};

        for(int i{10}; i > 0; --i)
        {
            cv_.wait_for(lock, time_ / 10);
            if (stop_)
                return;
        }

        f_();
    }

    std::chrono::milliseconds time_;
    std::function <void(void)> f_;

    std::mutex mtx_;
    std::condition_variable cv_{};
    std::thread wait_thread{[this]() { wait(); }};
    bool stop_ = false;

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
};

ChorderImpl::ChorderImpl(const Context* context)
    : c_(context)
{}

ChorderPtr Chorder::create(const Context* context)
{
    return ChorderImpl::create(context);
}

ChorderImpl::~ChorderImpl()
{}

void ChorderImpl::tryChord(const char c)
{

}

}

