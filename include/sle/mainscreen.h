#ifndef SLE_MAINSCREEN_H
#define SLE_MAINSCREEN_H

#include "sle/context.h"
#include "sle/customtypes.h"
#include "sle/point.h"
#include "sle/screen.h"

namespace sle
{

enum class ScreenState
{
    NONE,
    REFRESH,
    REDRAW,
};

class MainScreen
    : public Screen
{
public:
	MainScreen(
        const ContextPtr& context);

    ~MainScreen();

    void init();

    int getCh();

    /// Replace a character at given position, does not advance cursor.
    /// \returns False, if char could not be painted.
    bool paintAt(
        const char c,
        const PointPtr& point);

    /// Paints a character at current position and advances.
    ///
    /// \param c The char to paint at current cursor position.
    /// \param replace If true, replaces any previous character.
    /// \return False, if char could not be painted (e.g. line too long).
    bool paintCh(
        const char c,
        const bool replace = false);

    /// Paints supplied lines starting from specified point in buffer coords.
    void paint(
        const std::vector<std::string>& text,
        const PointPtr& point = Point::make(0, 0));

    const PointPtr& cursor() const;

    void refreshScr(
        const ScreenState state,
        const PointPtr& point = nullptr,
        const int count = 1);

    void test();

    /// Moves the cursor within the bounds of the screen.
    ///
    /// If the specified point is not within the screen, cursor does not move.
    /// \param point Point in the coordinate space of the screen.
    void moveCursor(
        const PointPtr& point);

    /// Srolls the screen up/down <lines> lines. Cursor is not moved.
    void scrollScr(
        const int lines,
        const Direction dir);

    /// Converts a point in buffer space to a point in the screen space.
    PointPtr toScrCoord(
        const PointPtr& bufCoord) const;

    /// Converts a point in screen space to a point in the buffer space.
    PointPtr toBufCoord(
        const PointPtr& scrCoord) const;

    /// Returns true if there is text after the cursor.
    bool isAtLineEnd() const;

    /// Returns the height of the window.
    int height() const;

    /// Returns true if the point is within the screen dimensions.
    bool isInsideScr(
        const PointPtr& point = nullptr) const;

    /// Deletes character, moves rest of line left.
    void delCh(
        const PointPtr& point = nullptr,
        const int count = 1);

    void clrToEol(
        const PointPtr& point = nullptr);

    /// Updates visible range if it has changed.
    ///
    /// \param lines Number of lines changed.
    /// \param bufCur Where buffer cursor will be after operation.
    void updateVisible(
        const int lines,
        const PointPtr& bufCur = nullptr);

    // Update visible range and clear now empty lines.
    //
    // \param cur Where cursor is now (if not updated yet).
    void clrEmptyLines();

private:
    void addNewline();

    ContextPtr c_ = nullptr;
    //CursePtr scr_ = nullptr; // ei tarvi enaa, screenista tulee nyt.
    int width_, height_;
    PointPtr cursor_ = Point::make(0, 0);
};

} // namespace sle

#endif // SLE_MAINSCREEN_H
