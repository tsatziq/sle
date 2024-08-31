#ifndef SLE_MODEBASE_H
#define SLE_MODEBASE_H

#include <memory>

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
    CHANGEEOL,
    CHANGELN,
    CHANGE,
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

class ModeBase
{
public:
    virtual ~ModeBase() = default;

    /// Take input and process them.
    /// \return true, if program should quit.
    virtual bool handle() = 0;
    // huom korvaa joskus bool enumilla.

    //ehk joskus virtual void setData() = 0;
protected:
    static bool quit_;
};

using ModePtr = std::shared_ptr<ModeBase>;

class ModeDataBase;
using ModeDataPtr = std::shared_ptr<ModeDataBase>;

/// Base class for passing data from one mode to another.
class ModeDataBase
{
public:
    virtual ~ModeDataBase() = default;
};

} // namespace sle

#endif // SLE_MODEBASE_H
