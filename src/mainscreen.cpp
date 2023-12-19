#include "sle/mainscreen.h"
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
    idlok(scr_, true);
    scrollok(scr_, true);
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
    case 'p':
        wmove(scr_, height_ - 1, 0);
        break;
    case 'o':
        waddch(scr_, '\n');
        break; 
    case 'l':
        wmove(scr_, height_, 0);
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
        
    // SEURAAVAKSI: tahan et pitkat rivit tulee \ vai mika onkaa symboli
    // ei wrappia. ja kato et newlinet tulee. ja paivita visiblerange ja
    // handlaa scrolli kun ruutu tayttyy.
}

void MainScreen::addNewline()
{
        mvwaddch(scr_, cursor_.y(), cursor_.x(), '\n');
        cursor_.set(0, cursor_.y() + 1);
    /*else
    {
        mvwaddch(scr_, cursor_.y(), cursor_.x(), '\n');
        cursor_.set(0, cursor_.y() + 1);
        wscrl(scr_, 1);
    }*/

}

/*
void MainScreen::paint(const std::vector<std::string>& text)
{
    for (std::string line : text) {
        wprintw(scr_, line.c_->tr());
    } 

    wrefresh(scr_);
}
*/

}
