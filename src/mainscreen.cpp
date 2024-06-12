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

        wclrtoeol(scr_);
        cursor_.set(0, cursor_.y() + 1);

        auto moveLines = c_->buf->getRange({
            toBufCoord(cursor_), 
            c_->visibleRange.end()});

        paint(moveLines, cursor_);
        moveCursor(cursor_);
    }
    else
    {
        c_->visibleRange.start().incY();
        c_->visibleRange.end().incY();

        auto moveLines = c_->buf->getRange(c_->visibleRange);

        paint(moveLines);
        moveCursor({0, cursor_.y()});
    };
}

void MainScreen::paint(
    const std::vector<std::string>& text,
    Point point)
{
    wmove(scr_, point.y(), point.x());
    point.setX(0);

    for (const auto& str : text)
    {
        wclrtoeol(scr_);
        wprintw(scr_, str.c_str());
        point.incY();
        wmove(scr_, point.y(), point.x());
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
