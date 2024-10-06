#include "sle/editloop.h"
#include "sle/cmdmode.h"
#include "sle/customtypes.h"
#include "sle/insertmode.h"
#include "sle/normalmode.h"
#include "sle/mainscreen.h"
#include <memory>
#include <sstream>
#include <string>
#include <cctype>
#include <queue>

namespace sle 
{

bool ModeBase::quit_ = false;

EditLoop::EditLoop(
    const ContextPtr& context)
    :
    c_(context)
{}

void EditLoop::run()
{
    modePool_[Mode::NORMAL] = std::make_shared<NormalMode>(this);
    modePool_[Mode::INSERT] = std::make_shared<InsertMode>(this);
    modePool_[Mode::CMD] = std::make_shared<CmdMode>(this);
    changeMode(Mode::NORMAL);
    //while (!mode_->handle())
        //mode_->handle();
    while (true)
    {
        bool quit = mode_->handle();
        if (quit)
            break;
    }
}

void EditLoop::changeMode(
    const Mode& newMode,
    ModeDataBase* data)
{
    mode_ = modePool_.at(newMode);

    switch (newMode)
    {
    case Mode::INSERT:
    {
        auto im = static_cast<EditLoop::InsertMode*>(mode_.get());
        im->setData(static_cast<InsertModeData*>(data));
        break;
    }
    case Mode::NORMAL:
    default:
        break;
    }
}

}
