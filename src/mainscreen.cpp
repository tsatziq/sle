#include "sle/mainscreen.h"
#include <iostream>

namespace sle
{

MainScreen::MainScreen()
{}

void MainScreen::init(int height, int width)
{
/*
	height_ = maxSize.height - CMDLINE_INIT_HEIGHT - MODELINE_HEIGHT;
	width_ = maxSize.width - SIDEBAR_WIDTH;
	screen_ = newwin(height_, width_, 0, SIDEBAR_WIDTH);
	
    actions_.on<PaintMainScr>([&](const PaintMainScr& data)
    {
        paint(data.txt);
    });
    
	wrefresh(screen_);
*/
}

MainScreen::~MainScreen()
{
	delwin(screen_);
}

void MainScreen::paint(const std::vector<std::string>& text)
{
    for (std::string line : text) {
        wprintw(screen_, line.c_str());
    } 

    wrefresh(screen_);
}
	
}
