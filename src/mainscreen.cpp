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
    scr_ = newwin(height_, width_, 0, 0);
}

int MainScreen::getCh()
{
    return wgetch(scr_);
}

bool MainScreen::paintCh(
    const char c)
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

        if (!atLineEnd)
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
    // HUOH siirry joskus kayttaa deleteln/insertln, ja muutenki turha tyo pois
 
    if (cursor_->y() < height_ - 1)
    {
        // Screen was not full.
        if (c_->buf->size() - 1 < height_)
            c_->visibleRange->end()->incY();

        wclrtoeol(scr_);
        cursor_->set(0, cursor_->y() + 1);

        auto moveLines = c_->buf->getRange(
            Range::make(toBufCoord(cursor_), c_->visibleRange->end()));

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
            wprintw(scr_, str.c_str());
            pTemp->incY();

            if (!isInsideScr(pTemp))
                break; 

            wmove(scr_, pTemp->y(), pTemp->x());
        }
    }
}

void MainScreen::test()
{
    wmove(scr_, 1,0);
    wclrtoeol(scr_);
    auto cur = c_->buf->cursor();
    wprintw(scr_, std::to_string(c_->buf->size()).c_str());

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
    wrefresh(scr_);
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
        wdelch(scr_);
    else
        mvwdelch(scr_, point->y(), point->x());
}

}
