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
    case Action::INSERT:
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
    case Action::CHANGE:
    {
        auto cur = scr_->cursor();
        auto r = Range::make(cur, data_.end());
        scr_->paintAt('$', data_.end());

        while (true)
        {
            char ch = scr_->getCh();
            
            switch (ch)
            {
            case 'q':
            {
                if (*cur <= *(data_.end()))
                {
                    data_.end()->incX();
                    delBufScr(Range::make(cur, data_.end()));
                }
                quitMode();
                return false;
            }
            case '\n':
            {
                auto cur = scr_->cursor();
                if (*cur <= *(data_.end()))
                {
                    data_.end()->incX();
                    delBufScr(Range::make(cur, data_.end()));
                }
                buf_->addCh(ch);
                scr_->paintCh(ch);
                break;
            }
            default:
                auto ln = buf_->getText().at(cur->y()); // DEBUG
                if (*(buf_->cursor()) <= *(data_.end()))
                {
                    if (scr_->paintCh(ch, true))
                       buf_->addCh(ch, true); 
                }
                else
                {
                    if (scr_->paintCh(ch))
                        buf_->addCh(ch);
                }
                ln = buf_->getText().at(cur->y()); // DEBUG
                auto i = 2; // DEBUG
                break;
            }
        }
        
        return false;
        break;
    }
    case Action::CHANGEEOL:
    {
        scr_->paintAt('$', data_.end());
        auto cur = scr_->cursor();
        buf_->erase(Range::make(cur, data_.end()));
        auto l = buf_->getText().at(0); // DEBUG

        while (true)
        {
            char ch = scr_->getCh();
            
            switch (ch)
            {
            case 'q':
            {
                if (cur->x() <= data_.end()->x())
                    scr_->clrToEol();
                auto ln = buf_->getText().at(0); // DEBUG
                quitMode();
                return false;
            }
            case '\n':
            {
                auto cur = scr_->cursor();
                if (cur->x() <= data_.end()->x())
                    scr_->clrToEol();
                buf_->addCh(ch);
                scr_->paintCh(ch);
                scr_->refreshScr(ScreenState::REDRAW, Point::make(0,0));
                data_.setType(Action::INSERT);
                return false;
            }
            default:
                if (scr_->paintCh(ch))
                    buf_->addCh(ch);
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
    scr_->delCh(nullptr, range->end()->x() - range->start()->x());
}

}
