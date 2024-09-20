#include "sle/mainscreen.h"
#include "sle/buffer.h"
#include <iostream>
#include <ncurses.h>

namespace sle
{

MainScreen::MainScreen(
    const ContextPtr& context)
    :
    c_(context)
{}

MainScreen::~MainScreen()
{
    delwin(scr_);
    endwin();
}

void MainScreen::init()
{
    initscr();
    noecho();
    getmaxyx(stdscr, height_, width_); 
    height_ = 23; // LLDB DEBUG!
    width_ = 80; // LLDB DEBUG!
    scr_ = newwin(height_, width_, 0, 0);
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

void MainScreen::addNewline()
{
    if (cursor_->y() < height_ - 1)
    {
        // Screen was not full.
        if (c_->buf->size() - 1 < height_)
            c_->visibleRange->end()->incY();

        wclrtoeol(scr_);
        cursor_->set(0, cursor_->y() + 1);

        auto moveLines = c_->buf->getRange(
            Range::make(toBufCoord(cursor_), c_->visibleRange->end()));

        //paint(moveLines, cursor_, Point::Type::SCRCOORD); DBEUG
        paint(moveLines, cursor_);
        moveCursor(cursor_);
    }
    else
    {
        c_->visibleRange->start()->incY();
        c_->visibleRange->end()->incY();

        auto moveLines = c_->buf->getRange(c_->visibleRange);

        paint(moveLines);
        auto p = Point::make(0, cursor_->y()); // debug
        moveCursor(Point::make(0, cursor_->y()));
    };
}

void MainScreen::paint(
    const std::vector<std::string>& text,
    const PointPtr& point)
{
    //if (type != Point::Type::SCRCOORD)
    //    pTemp = Point::make(toScrCoord(pTemp));

    if (!isInsideScr(point))
        return;

    PointPtr pTemp = Point::make(point); 
    wmove(scr_, pTemp->y(), pTemp->x());

    if (text.empty())
        return;
    
    if (text.size() == 1)
    {
        wclrtoeol(scr_);
        wprintw(scr_, text.front().c_str());
    }
    else
    {
        for (const auto& str : text)
        {
            wclrtoeol(scr_);
            mvwprintw(scr_, pTemp->y(), pTemp->x(), str.c_str());
            pTemp->incY();

            if (!isInsideScr(pTemp))
                break; 

            wmove(scr_, pTemp->y(), pTemp->x());
        }
    }

    doupdate();
    redrawwin(scr_);
    wnoutrefresh(scr_);
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
        wrefresh(scr_);
        break;
    case ScreenState::REDRAW:
        if (point)
            wredrawln(scr_, point->y(), count);
        else
            redrawwin(scr_);
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
    //laita tahan if point == cursor, nyt vaa se valitti jotai.
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
    moveCursor(cursor_); 
    c_->visibleRange = r;
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

}
