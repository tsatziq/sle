#ifndef SLE_SCREEN_H
#define SLE_SCREEN_H

#include <string>
#include <vector>
#include <memory>
#include <ncurses.h>
#include <panel.h>

namespace sle
{

using CursePtr = WINDOW*;
using PanelPtr = PANEL*;

// Refaktoroi et tas ois esim cursor(), paint? jne.
class Screen
{
public:
    Screen() = default;

    virtual ~Screen() = default;

    //virtual void init() = 0;

    //int getCh();

protected:
    PanelPtr panel_ = nullptr;
    CursePtr scr_ = nullptr;
    bool hidden_ = false;
};

} // namespace sle


#endif // SLE_SCREEN_H
