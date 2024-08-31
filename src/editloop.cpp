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
    const Mode& newMode)
{
    mode_ = modePool_.at(newMode);
    // jos teet sen ModeData, niin tee perus pure abstract ModeData* data() = 0
    // ja derived classis InsertModeData* data() override. toimii!
}

}
