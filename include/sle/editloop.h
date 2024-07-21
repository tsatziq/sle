#ifndef SLE_EDITLOOP_H
#define SLE_EDITLOOP_H

#include "sle/context.h"
#include "sle/buffer.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <queue>

namespace sle
{

enum class Mode
{
    NORMAL,
    INSERT,
};

struct LongCmd
{
    int count = 1;
    char cmd;
    std::queue<char> to;

    static std::shared_ptr<LongCmd> make()
    {
        return std::make_shared<LongCmd>();
    }
};

using LongCmdPtr = std::shared_ptr<LongCmd>;

class EditLoop
{
public:
    EditLoop(
        const ContextPtr& context);

    void init();

private:
    /* TODO / SEURAAVAKS: cmd modessa myos pitaa ottaa yksitelle inputtia
        koska joskus halutta esim erase. eli sinneki start/continuecmd.
        ehk tee myos static bool quit_ ModeBaseen, alusta cpp alus.
    */
    class ModeBase
    {
    public:
        virtual ~ModeBase() = default;

        /// Take input and process them.
        /// \return true, if program should quit.
        virtual bool handle() = 0;
        // huom korvaa joskus bool enumilla.
    protected:
        static bool quit_;
    };

    using ModePtr = std::shared_ptr<ModeBase>;

    /// Represents if command was finished or not.
    enum class CmdState
    {
        UNFINISHED,
        FINISHED,
    };

    void changeMode(
        const Mode& mode);

    class NormalMode;
    class InsertMode;

    ContextPtr c_ = nullptr;
    ModePtr mode_ = nullptr;
    std::unordered_map<Mode, ModePtr> modePool_;
    LongCmdPtr prevCmd_ = nullptr;
};

} // namespace sle

#endif // SLE_EDITLOOP_H
