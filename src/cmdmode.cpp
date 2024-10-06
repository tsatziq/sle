#include "sle/cmdmode.h"
#include "sle/cmdline.h"

namespace sle
{

EditLoop::CmdMode::CmdMode(
    EditLoop* parent)
    :
    parent_(parent),
    c_(parent->c_),
    cln_(c_->cln)
{}

bool EditLoop::CmdMode::handle()
{
    switch (data_.type())
    {
    case Action::CMDMODE:
    {
        cln_->paintCh(':');
        cln_->update();

        while (true)
        {
            char c = cln_->getCh();

            switch (c)
            {
            case '0x03':
                cln_->clear();
                cln_->update();
                parent_->changeMode(Mode::NORMAL);
                return false;
            default:
                break;
            }
        }
        return false;
        break;
    }
    default:
        return false;
    }
    return true;
}

}
