#include "sle/mainscreen.h"
#include "sle/buffer.h"
#include <iostream>
#include <ncurses.h>

namespace sle
{

MainScreen::MainScreen(
    const ContextPtr& context)
    :
    Screen(),
    c_(context)
{}

MainScreen::~MainScreen()
{
    del_panel(panel_);
    endwin();
}

void MainScreen::init()
{
    initscr();
    noecho();
    getmaxyx(stdscr, height_, width_); 
    height_ = 22; // LLDB DEBUG!
    width_ = 80; // LLDB DEBUG!
    scr_ = newwin(height_, width_, 0, 0);
    panel_ = new_panel(scr_);
}

int MainScreen::getCh()
{
    return wgetch(scr_);
}

// SEURAAVAKS: laita et mainscreen ottaa kaikki bufcoordeina ja ite vaihtaa.
// kuitenki suurin osa kayttaa niita..
bool MainScreen::paintAt(
    const char c,
    const PointPtr& point)
{
    if (!point)
        point->set(cursor_->x(), cursor_->y());
    if (!isInsideScr(point))
        return false;

    switch (c)
    {
    case '\n':
        addNewline();
        return true;
    default:
        if (point->x() == width_ - 1)
            return false;

        mvwaddch(scr_, point->y(), point->x(), c);
        wmove(scr_, cursor_->y(), cursor_->x());

        return true;
    }
}

bool MainScreen::paintCh(
    const char c,
    const bool replace)
{
    auto lineLen = c_->buf->lineLen(toBufCoord(cursor_));
    bool atLineEnd = isAtLineEnd(); // True, if there is no text after cursor.
        
    switch (c)
    {
    case '\n':
        addNewline();
        return true;
    default:
        if (lineLen == width_ - 1)
            return false;

        if (!atLineEnd && !replace)
            mvwinsch(scr_, cursor_->y(), cursor_->x(), c);
        else
            mvwaddch(scr_, cursor_->y(), cursor_->x(), c);
        cursor_->incX();

        if (!atLineEnd && (cursor_->x() != width_ - 1))
            moveCursor(cursor_);
        return true;
    }
}

// NOTE: palauta decX jne ref siihen, et voi antaa suoraa funkkarille.
void MainScreen::addNewline()
{
    auto prevCur = Point::make(c_->buf->cursor());
    prevCur->decY();

    if (toScrCoord(prevCur)->y() < height_ - 1)
    {
        if (c_->buf->size() - 2 < height_)
            c_->visibleRange->end()->incY();

        auto txt = c_->buf->getRange(Range::make(
            prevCur,
            c_->visibleRange->end()));

        cursor_->setX(0);
        paint(txt, cursor_);
        cursor_->setY(cursor_->y() + 1);
    }
    else
        scrollScr(1, Direction::DOWN);

    cursor_->setX(0);
    moveCursor(cursor_);
}

void MainScreen::paint(
    const std::vector<std::string>& text,
    const PointPtr& point)
{
    if (!isInsideScr(point))
        return;

    PointPtr pTemp = Point::make(point); 
    wmove(scr_, pTemp->y(), pTemp->x());

    if (text.empty())
        return;

    for (const auto& str : text)
    {
        wclrtoeol(scr_);
        mvwprintw(scr_, pTemp->y(), pTemp->x(), str.c_str());
        pTemp->incY();

        if (!isInsideScr(pTemp))
            break; 

        wmove(scr_, pTemp->y(), pTemp->x());
    }

    update_panels();
    doupdate();
}

const PointPtr& MainScreen::cursor() const
{
    return cursor_;
}

void MainScreen::refreshScr(
    const ScreenState state,
    const PointPtr& point,
    const int count)
{
    switch (state)
    {
    case ScreenState::REFRESH:
        update_panels();
        doupdate();
        break;
    case ScreenState::REDRAW:
        if (point)
            wredrawln(scr_, point->y(), count);
        else
        {
            update_panels();
            doupdate();
        }
        break;
    default:
        break;
    };
}

void MainScreen::test()
{
    wmove(scr_, 1,0);
    wclrtoeol(scr_);
    wprintw(scr_, "moi");

}

void MainScreen::moveCursor(
    const PointPtr& point)
{
    if (point->x() < 0 || point->y() < 0)
        return;
    if (point->x() > width_ || point->y() > height_)
        return;
    cursor_ = point;
    wmove(scr_, point->y(), point->x());
}

void MainScreen::scrollScr(
    const int lines,
    const Direction dir)
{
    if (c_->buf->size() < height_)
        return;

    auto r = c_->visibleRange;
    auto top = r->start();
    auto end = r->end();

    switch (dir)
    {
    case Direction::DOWN:
    {
        top->setY(top->y() + lines);
        end->setY(end->y() + lines);

        if (end->y() > c_->buf->size() - 1)
        {
            end->setY(c_->buf->size() - 1);
            top->setY(end->y() + 1 - height_);
        }
        break;
    }
    case Direction::UP:
    {
        top->setY(top->y() - lines);
        end->setY(end->y() - lines);

        if (top->y() < 0)
        {
            top->setY(0);
            end->setY(height_ - 1);
        }
        break;
    }
    default:
        return;
    }

    paint(c_->buf->getRange(c_->visibleRange));
    c_->visibleRange = r;
    auto s = r->start(); // DEBUG
    auto e = r->end(); // DEBUG
    auto i = 1; // DEBUG;
}

PointPtr MainScreen::toScrCoord(
    const PointPtr& bufCoord) const
{
    int scrTopLine = c_->visibleRange->start()->y();
    return Point::make(bufCoord->x(), bufCoord->y() - scrTopLine);
}

PointPtr MainScreen::toBufCoord(
    const PointPtr& scrCoord) const
{
    int scrTopLine = c_->visibleRange->start()->y();
    return Point::make(scrCoord->x(), scrCoord->y() + scrTopLine);
}

bool MainScreen::isAtLineEnd() const
{      
    return c_->buf->lineLen(toBufCoord(cursor_)) < cursor_->x();
}

int MainScreen::height() const
{
    return height_;
}

bool MainScreen::isInsideScr(
    const PointPtr& point) const
{
    if (!point->isFullySet())
        point |= cursor_;

    if (point->y() < 0 || point->y() > height_ - 1)
        return false;
    if (point->x() < 0 || point->x() > width_ - 1)
        return false;

    return true;
}

void MainScreen::delCh(
    const PointPtr& point,
    const int count)
{
    if (!point || point->isUnset())
        for (int i = 0; i < count; ++i)
            wdelch(scr_);
    else
        for (int i = 0; i < count; ++i)
            mvwdelch(scr_, point->y(), point->x());
}

void MainScreen::clrToEol(
    const PointPtr& point)
{
    if (point)
    {
        auto p = Point::make(toScrCoord(point));
        wmove(scr_, p->y(), p->x());
    }

    wclrtoeol(scr_);
    moveCursor(cursor_);
}

// saisko noi parametrit korvattua rangella?
void MainScreen::updateVisible(
    const int lines,
    const PointPtr& bufCur)
{
    PointPtr tmpCur = Point::make(bufCur);
    if (!bufCur)
        tmpCur = Point::make(c_->buf->cursor());

    int lastLn = c_->visibleRange->end()->y();
    int firstLn = c_->visibleRange->start()->y();

    if (c_->visibleRange->lines() < height() - 1)
        c_->visibleRange->end()->setY(lastLn + lines);

    if (tmpCur->y() < firstLn)
    {
        c_->visibleRange->start()->setY(tmpCur->y());
        c_->visibleRange->end()->setY(tmpCur->y() + height() - 1);
    }
    
    // Check buf has enough lines.
    if (c_->visibleRange->end()->y() > c_->buf->size() - 1)
        c_->visibleRange->end()->setY(c_->buf->size() - 1);

    clrEmptyLines(); // alussa kutsuin tata normalmodessa
}

void MainScreen::clrEmptyLines()
{
    if (c_->visibleRange->lines() < height_)
    {
        int ln = toScrCoord(c_->visibleRange->end())->y();
        for (int i = ln + 1; i <= height_ - 1; ++i)
        {
            wmove(scr_, i, 0);
            wclrtoeol(scr_);
        }
    }
}

}
