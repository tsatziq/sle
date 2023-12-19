#include "sle/mainscreen.h"
#include <iostream>
#include <ncurses.h>

namespace sle
{

MainScreen::MainScreen(
    const Context& context)
    :
    c_(context)
{}

void MainScreen::init(
    int height,
    int width)
{
/*
	height_ = maxSize.height - CMDLINE_INIT_HEIGHT - MODELINE_HEIGHT;
	width_ = maxSize.width - SIDEBAR_WIDTH;
	scr_ = newwin(height_, width_, 0, SIDEBAR_WIDTH);
	
    actions_.on<PaintMainScr>([&](const PaintMainScr& data)
    {
        paint(data.txt);
    });
    
	wrefresh(scr_);
*/
}

MainScreen::~MainScreen()
{
    delwin(scr_);
}

/*
void MainScreen::paint(const std::vector<std::string>& text)
{
    for (std::string line : text) {
        wprintw(scr_, line.c_str());
    } 

    wrefresh(scr_);
}
*/

}
