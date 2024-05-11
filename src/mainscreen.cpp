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

void MainScreen::paintCh(
    const char c)
{
    switch (c)
    {
    case '\n':
        addNewline();
        break;
    default:
        if (cursor_.x() < width_ - 2)
        {
            mvwaddch(scr_, cursor_.y(), cursor_.x(), c);
            cursor_.incX();
        }
        else if (cursor_.x() == width_ - 2)
        {
            mvwaddch(scr_, cursor_.y(), width_ - 2, '$');
            cursor_.incX();
        }
        break;
    }
}

void MainScreen::addNewline()
{
    if (cursor_.y() < height_ - 1)
    {
        mvwaddch(scr_, cursor_.y(), cursor_.x(), '\n');
        cursor_.set(0, cursor_.y() + 1);
        c_->visibleRange.end().incY();
    }
    else
    {
        c_->visibleRange.start().incY();
        werase(scr_);
        paint(c_->buf->getRange(c_->visibleRange));
        c_->visibleRange.end().incY();
        int x, y;
        getyx(scr_, y, x);
        cursor_.set(x, y);
    }
}

void MainScreen::paint(const std::vector<std::string>& text)
{
    wmove(scr_, 0, 0);
    for (const auto& str : text)
        wprintw(scr_, str.c_str());
}

void MainScreen::test()
{
    werase(scr_);

    std::vector<std::string> v =
    {
        "heipp vaan taas",
        "kokeillaan onnistuisko tama jotenkin nyt",
        "ehkapa viela muutama pieni rivi",
        "ja sitten saa hetkeksi riittaa."
    };

    paint(v);
    int x, y;
    getyx(scr_, y, x);
    cursor_.set(x, y);
}

}
