#ifndef SLE_MODELOOP_H
#define SLE_MODELOOP_H

#include "sle/context.h"
#include "sle/screen.h"

namespace sle {

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
    static ModeLoopPtr create(
        ContextPtr context, const std::string& file);

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
