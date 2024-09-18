#ifndef SLE_NORMALMODE_H
#define SLE_NORMALMODE_H

#include "sle/editloop.h"
#include "sle/modebase.h"

namespace sle
{

class EditLoop::NormalMode
    : public ModeBase
{
public:
    explicit NormalMode(
        EditLoop* parent);

    bool handle() override;

private:
    enum class CmdPart
    {
        NONE,
        COUNT,
        ACTION,
        MOTION,
        TO,
    };

    /// Parse the current input string.
    /// \param input The string to parse.
    /// \returns CmdState enum indicating if command was finished.
    CmdState parse(
        const char c);

    void execute(
        const Command& cmd);

    bool isPrimaryAct(
        const Action a) const;

    bool isPendingCmd(
        const Motion m) const;

    bool isMotion(
        const char c) const;

    /// E.g. 'dd' and 'cc'.
    bool isDoubleCmd(
        const Action action) const;

    Motion toMotion(
        const char c);

    Action toAction(
        const char c);

    EditLoop* parent_ = nullptr;
    ContextPtr c_ = nullptr;
    Buffer* b_ = nullptr;
    MainScreen* s_ = nullptr;

    bool exitMode_ = false;
    std::string input_;
    static Command prevCmd_; ///< Last valid command for repeat.
    Command cmd_; ///< Command that is currently being formed.
    CmdPart prevPart_ = CmdPart::NONE; ///< Which part was parsed last.
};

} // namespace sle

#endif // SLE_NORMALMODE_H
