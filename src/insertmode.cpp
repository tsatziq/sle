#include "sle/insertmode.h"
#include "sle/mainscreen.h"

namespace sle
{

EditLoop::InsertMode::InsertMode(
    EditLoop* parent)
    :
    parent_(parent),
    c_(parent->c_)
{}

bool EditLoop::InsertMode::handle()
{
    switch (data_.type)
    {
    case InsertType::BASIC:
    {
        while (true)
        {
            char ch = c_->scr->getCh();
            
            switch (ch)
            {
            case 'q':
            {
                quitMode();
                return false;
            }
            case '\n':
                c_->buf->addCh(ch);
                c_->scr->paintCh(ch);
                return false;
            default:
                if (c_->scr->paintCh(ch))
                    c_->buf->addCh(ch);
                return false;
            }
        }
        break;
    }
    case InsertType::CHANGE:
    {
        /*
        c_->scr->paintAt('$', cEnd_);
        while (true)
        {
            char ch = c_->scr->getCh();
            
            switch (ch)
            {
            case 'q':
            {
                quitMode();
                return false;
            }
            case '\n':
                c_->buf->addCh(ch);
                c_->scr->paintCh(ch);
                return false;
            default:
                if (c_->buf->cursor()->x() <= cEnd_->x())
                    if (c_->scr->paintCh(ch, true))
                       c_->buf->addCh(ch, true); 
                if (c_->scr->paintCh(ch))
                    c_->buf->addCh(ch);
                return false;
            }
        }
        */
        return false;
        break;
    }
    default:
        return false;
    }
}

void EditLoop::InsertMode::quitMode()
{
    auto cursor = c_->buf->move(Direction::LEFT);
    c_->scr->moveCursor(c_->scr->toScrCoord(cursor));
    parent_->changeMode(Mode::NORMAL);
}

}
