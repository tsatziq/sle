#ifndef SLE_MAINSCREEN_H
#define SLE_MAINSCREEN_H

#include "sle/context.h"
#include "sle/point.h"
#include "sle/screen.h"

namespace sle
{

class MainScreen
{
public:
	MainScreen(
        const ContextPtr& context);

    ~MainScreen();

    void init();

    int getCh();

    /// Paints a character at current position of screen cursor.
    void paintCh(
        const char c);

private:
    void addNewline();

    ContextPtr c_ = nullptr;
    CursePtr scr_ = nullptr;
    int width_, height_;
    Point cursor_;
};

} // namespace sle

#endif // SLE_MAINSCREEN_H
