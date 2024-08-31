#ifndef SLE_EDITLOOP_H
#define SLE_EDITLOOP_H

#include "sle/context.h"
#include "sle/buffer.h"
#include "sle/modebase.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <queue>

namespace sle
{

/// Represents if command was finished or not.
enum class CmdState
{
    UNFINISHED,
    FINISHED,
    INVALID,
};

class EditLoop
{
public:
    EditLoop(
        const ContextPtr& context);

    void run();

private:
    void changeMode(
        const Mode& mode,
        ModeDataBase* data = nullptr); 

    class NormalMode;
    class InsertMode;

    ContextPtr c_ = nullptr;
    ModePtr mode_ = nullptr;
    std::unordered_map<Mode, ModePtr> modePool_;
};

} // namespace sle

#endif // SLE_EDITLOOP_H
