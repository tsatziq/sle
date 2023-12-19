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
        default:
            c_->scr->paintCh(ch);
            c_->buf->addCh(ch);
            break;
        }

        if (endLoop)
            break;
    }
}

}
