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

    /// Paints lines from start of the screen.
    void paint(const std::vector<std::string>& text);

    void test();

private:
    void addNewline();

    ContextPtr c_ = nullptr;
    CursePtr scr_ = nullptr;
    int width_, height_;
    Point cursor_ = Point(0, 0);
};

} // namespace sle

#endif // SLE_MAINSCREEN_H
