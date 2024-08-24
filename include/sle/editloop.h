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

enum class Motion
{
    NONE,
    BEGINLN,
    BOTTOM,
    ENDLN,
    LEFT,
    MIDDLE,
    MIDDLELN,
    RIGHT,
    UP,
    DOWN,
    SCRFWD,
    SCRBCK,
    TILL,
    TILLBCK,
    TO,
    TOBCK,
    TOP,
    LINE, ///< Whole line commands, e.g. 'dd'.
    TOLINE,
};

/*
-h,l,b korvaa h,l,m. sit saa control versiot kaikista
-ctrl k completioniin. ois hyva jos vaik u keskeyttais sen
    ja palais siihen sanajuureen.
*/
enum class Action
{
    NONE,
    APPEND,
    DELCHAR,
    DELETE,
    INSERT,
    TEST,
    WRITE,
    WRITEQUIT,
    QUIT,
};

struct Command
{
    std::string count = "1";
    Action action;
    Motion motion = Motion::NONE;
    std::string to;

    static std::shared_ptr<Command> make()
    {
        return std::make_shared<Command>();
    }
};

using CommandPtr = std::shared_ptr<Command>;

class EditLoop
{
public:
    EditLoop(
        const ContextPtr& context);

    void run();

private:
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
        INVALID,
    };

    void changeMode(
        const Mode& mode);

    class NormalMode;
    class InsertMode;

    ContextPtr c_ = nullptr;
    ModePtr mode_ = nullptr;
    std::unordered_map<Mode, ModePtr> modePool_;
};

} // namespace sle

#endif // SLE_EDITLOOP_H
