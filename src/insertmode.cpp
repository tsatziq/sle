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
            default:
                buf_->addCh(ch);
                scr_->paintCh(ch);
                break;
            }
        }
        break;
    }
    case Action::CHANGE:
    {
        buf_->setCursor(data_.from());
        auto cur = buf_->cursor();
        auto r = Range::make(cur, data_.to());
        scr_->paintAt('$', scr_->toScrCoord(data_.to()));

        while (true)
        {
            char ch = scr_->getCh();
            
            switch (ch)
            {
            case 'q':
            {
                auto s = buf_->getText().at(0); // DEBUG
                if (*cur <= *(data_.to()))
                {
                    data_.to()->incX();
                    delBufScr(Range::make(cur, data_.to()));
                }
                s = buf_->getText().at(0); // DEBUG
                quitMode();
                return false;
                break;
            }
            case '\n':
            {
                auto cur = buf_->cursor();
                if (cur->y() < data_.to()->y())
                {
                    auto lnEnd = Point::make(buf_->lineLen() - 1, cur->y());
                    delBufScr(Range::make(cur, lnEnd));
                    cur->set(0, cur->y() + 1);
                    buf_->setCursor(Point::make(cur));
                    scr_->moveCursor(scr_->toScrCoord(cur)); // toScrCoord?
                    auto i = 1; // DEBUG
                }
                else if (*cur < *(data_.to()))
                {
                    data_.to()->incX();
                    delBufScr(Range::make(cur, data_.to()));
                    buf_->addCh(ch);
                    scr_->paintCh(ch);
                }
                else
                {
                    buf_->addCh(ch);
                    scr_->paintCh(ch);
                    data_.setType(Action::INSERT);
                    return false;
                }
                auto i = 1; // DEBUG
                break;
            }
            default:
            {
                auto ln = buf_->getText().at(cur->y()); // DEBUG
                if (*(buf_->cursor()) <= *(data_.to()))
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
        }
        
        return false;
        break;
    }
    // JOSKUS: tata ei kayteta nyt! kato voiko poistaa.
    case Action::CHANGEEOL:
    {
        scr_->paintAt('$', scr_->toScrCoord(data_.to()));
        buf_->setCursor(data_.from());
        auto cur = buf_->cursor();
        buf_->erase(Range::make(cur, data_.to()));
        auto l = buf_->getText().at(0); // DEBUG

        while (true)
        {
            char ch = scr_->getCh();
            
            switch (ch)
            {
            case 'q':
            {
                if (cur->x() <= data_.to()->x())
                    scr_->clrToEol();
                auto ln = buf_->getText().at(0); // DEBUG
                quitMode();
                return false;
            }
            case '\n':
            {
                if (cur->x() <= data_.to()->x())
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
    {
        return false;
    }
    }
}

void EditLoop::InsertMode::setData(
    InsertModeData* data)
{
    if (!data)
    {
        data_ = InsertModeData();
        return;
    }

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
    if (range->lines() == 1)
        scr_->delCh(nullptr, range->end()->x() - range->start()->x());
    else if (range->lines() > 1)
    {
        scr_->updateVisible(range->lines() - 1, range->end());
        auto txt = buf_->getRange(Range::make(
            Point::make(0, range->start()->y()),
            Point::make(0, c_->visibleRange->end()->y())));

        scr_->paint(
            txt,
            scr_->toScrCoord(Point::make(0, range->start()->y())));
    }
}

}
