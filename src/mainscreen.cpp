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
            mvwinsch(scr_, cursor_.y(), cursor_.x(), c);
        else
            mvwaddch(scr_, cursor_.y(), cursor_.x(), c);
        cursor_.incX();

        if (!atLineEnd && (cursor_.x() != width_ - 1))
            moveCursor(cursor_);
        return true;
    }
}

void MainScreen::addNewline()
{
    // HUOH siirry joskus kayttaa deleteln/insertln, ja muutenki turha tyo pois
 
    if (cursor_.y() < height_ - 1)
    {
        // Screen was not full.
        if (c_->buf->size() - 1 < height_)
            c_->visibleRange.end().incY();

        auto moveLines = c_->buf->getRange({
            toBufCoord(Point(0, cursor_.y())), 
            c_->visibleRange.end()});

        Point cursorPos = cursor_;
        int i = cursor_.y();
        for (const auto& line : moveLines)
        {
            moveCursor({0, i});
            wclrtoeol(scr_);
            mvwaddstr(scr_, i, 0, line.c_str());
            ++i;
        }
        
        cursorPos.set(0, cursorPos.y() + 1);
        moveCursor(cursorPos);
    }
    else
    {
        c_->visibleRange.start().incY();
        c_->visibleRange.end().incY();

        auto moveLines = c_->buf->getRange(c_->visibleRange);
        int i = 0;
        for (const auto& line : moveLines)
        {
            moveCursor({0, i});
            wclrtoeol(scr_);
            mvwaddstr(scr_, i, 0, line.c_str());
            ++i;
        }

        cursor_.set(0, cursor_.y());
        moveCursor(cursor_);
    };
}

void MainScreen::paint(const std::vector<std::string>& text)
{
    wmove(scr_, 0, 0);
    for (const auto& str : text)
    {
        wclrtoeol(scr_);
        wprintw(scr_, str.c_str());
    }
}

void MainScreen::test()
{
    for (int i = 0; i < height_; ++i)
        mvwaddstr(scr_, i, 0, "joku teksti");

    moveCursor({0, 0});
    wdeleteln(scr_);
    winsertln(scr_);
}

void MainScreen::moveCursor(
    const Point& point)
{
    //laita tahan if point == cursor, nyt vaa se valitti jotai.
    if (point.x() < 0 || point.y() < 0)
        return;
    if (point.x() > width_ || point.y() > height_)
        return;
    cursor_ = point;
    wmove(scr_, point.y(), point.x());
}

Point MainScreen::toScrCoord(
    const Point& bufCoord) const
{
    int scrTopLine = c_->visibleRange.start().y();
    return Point(bufCoord.x(), bufCoord.y() - scrTopLine);
}

Point MainScreen::toBufCoord(
    const Point& scrCoord) const
{
    int scrTopLine = c_->visibleRange.start().y();
    return Point(scrCoord.x(), scrCoord.y() + scrTopLine);
}

bool MainScreen::isAtLineEnd() const
{      
    return c_->buf->lineLen(toBufCoord(cursor_)) < cursor_.x();
}

}
