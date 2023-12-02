#ifndef SLE_MAINSCREEN_H
#define SLE_MAINSCREEN_H

#include "sle/screen.h"

namespace sle
{

class MainScreen
{
public:
	MainScreen();

    ~MainScreen() = default;

    void init(int height, int width);

    void paint(const std::vector<std::string>& text) = 0;

private:
    CursePtr screen_ = nullptr_;
    int width_, height_;

};

} // namespace sle

#endif // SLE_MAINSCREEN_H
