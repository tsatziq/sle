#include "sle/insertmode.h"
#include "sle/mainscreen.h"
#include "sle/point.h"

namespace sle
{

EditLoop::InsertMode::InsertMode(
    EditLoop* parent)
    :
    parent_(parent),
    c_(parent->c_),
    buf_(c_->buf),
    scr_(c_->scr)
{}

bool EditLoop::InsertMode::handle()
{
    switch (data_.type())
    {
    case InsertType::BASIC:
    {
        while (true)
        {
            char ch = scr_->getCh();
            
            switch (ch)
            {
            case 'q':
                quitMode();
                return false;
            case '\n':
                buf_->addCh(ch);
                scr_->paintCh(ch);
                break;
            default:
                if (scr_->paintCh(ch))
                    buf_->addCh(ch);
                break;
            }
        }
        break;
    }
    case InsertType::CHANGE:
    {
        scr_->paintAt('$', data_.end());
        while (true)
        {
            char ch = scr_->getCh();
            
            switch (ch)
            {
            case 'q':
            {
                auto cur = scr_->cursor();
                if (cur->x() <= data_.end()->x())
                    delBufScr(Range::make(cur, data_.end()));
                quitMode();
                return false;
            }
            case '\n':
            {
                auto cur = scr_->cursor();
                if (cur->x() <= data_.end()->x())
                    delBufScr(Range::make(cur, data_.end()));
                buf_->addCh(ch);
                scr_->paintCh(ch);
                break;
            }
            default:
                if (buf_->cursor()->x() <= data_.end()->x())
                {
                    if (scr_->paintCh(ch, true))
                       buf_->addCh(ch, true); 
                }
                else
                {
                    if (scr_->paintCh(ch))
                        buf_->addCh(ch);
                }
                break;
            }
        }
        
        return false;
        break;
    }
    default:
        return false;
    }
}

void EditLoop::InsertMode::setData(
    InsertModeData* data)
{
    if (!data)
        return;

    data_ = *data;
    delete data;
}

void EditLoop::InsertMode::quitMode()
{
    auto cursor = buf_->move(Direction::LEFT);
    scr_->moveCursor(scr_->toScrCoord(cursor));
    parent_->changeMode(Mode::NORMAL);
}

void EditLoop::InsertMode::delBufScr(
    const RangePtr& range)
{
    buf_->erase(range);
    scr_->delCh(nullptr, range->end()->x() - range->start()->x() + 1);
}

}
