#ifndef SLE_CMDMODE_H
#define SLE_CMDMODE_H

#include "sle/cmdmodelexer.h"
#include "sle/editloop.h"
#include "sle/modebase.h"
#include <memory>
#include <string>
#include <vector>

namespace sle
{

class CmdModeData
    : public ModeDataBase
{
public:
    CmdModeData() = default;

    CmdModeData(
        Action type)
        :
        type_(type)
    {}

    Action type() const
    {
        return type_;
    }

private:
    Action type_ = Action::CMDMODE;
};

using CmdModeDataPtr = std::shared_ptr<CmdModeData>;

class EditLoop::CmdMode
    : public ModeBase
{
public:
    explicit CmdMode(
        EditLoop* parent);

    bool handle() override;

    void parse(
        const std::string& cmd);

private:
    EditLoop* parent_ = nullptr;
    ContextPtr c_ = nullptr;
    CmdLine* cln_ = nullptr;
    CmdModeLexer lex_;
    CmdModeData data_;
    std::vector<std::string> txt_; // vaiha et kayta buf ja screen mis voi.
};

} // namespace sle

#endif // SLE_CMDMODE_H
