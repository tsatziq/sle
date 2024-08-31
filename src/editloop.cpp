#include "sle/editloop.h"
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
    changeMode(Mode::NORMAL);
    while (!mode_->handle())
        mode_->handle();
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
        auto d = static_cast<InsertModeData*>(data);
        im->setData(d);
        break;
    }
    case Mode::NORMAL:
    default:
        break;
    }
}

}
