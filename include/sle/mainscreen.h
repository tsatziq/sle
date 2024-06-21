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
    /// \return False, if char could not be painted (e.g. line too long).
    bool paintCh(
        const char c);

    /// Paints supplied lines starting from specified point.
    void paint(
        const std::vector<std::string>& text,
        Point point = Point(0, 0));

    void test();

    /// Moves the cursor within the bounds of the screen.
    ///
    /// If the specified point is not within the screen, cursor does not move.
    /// \param point Point in the coordinate space of the screen.
    void moveCursor(
        const Point& point);

    /// Converts a point in buffer space to a point in the screen space.
    Point toScrCoord(
        const Point& bufCoord) const;

    /// Converts a point in screen space to a point in the buffer space.
    Point toBufCoord(
        const Point& scrCoord) const;

    /// Returns true if there is text after the cursor.
    bool isAtLineEnd() const;

    /// Deletes character, moves rest of line left.
    void delCh(
        const Point& point = Point{},
        const int count = 1);

private:
    void addNewline();

    ContextPtr c_ = nullptr;
    CursePtr scr_ = nullptr;
    int width_, height_;
    Point cursor_ = Point(0, 0);
};

} // namespace sle

#endif // SLE_MAINSCREEN_H
