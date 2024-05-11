#include "sle/editloop.h"
#include "sle/mainscreen.h"
#include <memory>

namespace sle 
{

EditLoop::EditLoop(
    const ContextPtr& context)
    :
    c_(context)
{}

void EditLoop::init()
{
    auto scr = c_->scr;
    bool endLoop = false;

    while (true)
    {
        char ch = scr->getCh();
        
        switch (ch)
        {
        case 'q':
            endLoop = true;
            break;
        case 'z':
            c_->scr->test();
            break;
        default:
            c_->buf->addCh(ch);
            c_->scr->paintCh(ch);
            break;
        }

        if (endLoop)
            break;
    }
}

}
