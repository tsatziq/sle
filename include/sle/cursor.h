#ifndef SLE_CURSOR_H
#define SLE_CURSOR_H

#include "sle/coord.h"
#include <memory>

namespace sle {

class Context;
class Buffer;
class ScreenManager;

class Cursor;
using CursorPtr = std::unique_ptr<Cursor>;

enum class QuickMove
{
    H,
    L,
    M,
    B,
    C,
    E,
};

class Cursor
{
public:
    static CursorPtr create(const Context* context);

    virtual ~Cursor() = default;

    virtual void move(const Coord txtPos, const Coord scrPos) = 0;

    virtual void move(const Coord txtPos) = 0;

    virtual void move(const QuickMove pos) = 0;

    virtual void redraw() = 0;

    virtual Coord coord() const = 0;

    virtual void upDown(const int count) = 0;

    virtual void leftRight(const int count) = 0;

    /// Sets the y text position without moving screen position.
    /// Used after lines have been removed to correct the line number.
    virtual void setY(const int val) = 0;

protected:
    Cursor() = default;

private:
    Cursor(const Cursor&) = delete;

    Cursor& operator=(const Cursor&) = delete;
};

} // namespace sle

#endif // SLE_CURSOR_H
