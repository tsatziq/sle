#ifndef SLE_MODELOOP_H
#define SLE_MODELOOP_H

#include "sle/screen.h"

namespace sle {

class Context;
class Buffer;
class ScreenManager;

class ModeLoop;
using ModeLoopPtr = std::unique_ptr<ModeLoop>;

enum class Mode {
    normal,
    insert,
    cmd,
    quit,
};

class ModeLoop
{
public:
    static ModeLoopPtr create(const Context* context);

    virtual ~ModeLoop() = default;

    virtual void start() = 0;

    virtual void normalLoopRun() = 0;

    virtual void insertLoopRun() = 0;

    virtual void cmdLoopRun() = 0;

protected:
    ModeLoop() = default;

private:
    ModeLoop(const ModeLoop&) = delete;

    ModeLoop& operator=(const ModeLoop&) = delete;
};

} // namespace sle

#endif // SLE_MODELOOP_H
