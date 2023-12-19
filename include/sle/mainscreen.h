#ifndef SLE_MAINSCREEN_H
#define SLE_MAINSCREEN_H

#include "sle/context.h"
#include "sle/screen.h"

namespace sle
{

class MainScreen
{
public:
	MainScreen(
        const Context& context);

    ~MainScreen();

    void init(
        int height,
        int width);

private:
    Context c_;
    CursePtr scr_ = nullptr;
    int width_, height_;
};

} // namespace sle

#endif // SLE_MAINSCREEN_H
