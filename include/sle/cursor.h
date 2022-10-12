#ifndef SLE_CURSOR_H
#define SLE_CURSOR_H

#include "sle/types.h"
#include <memory>

namespace sle {

class Context;
class Buffer;
class ScreenManager;

class Cursor;
using CursorPtr = std::unique_ptr<Cursor>;

class Cursor
{
public:
    static CursorPtr create(const Context* context);

    virtual ~Cursor() = default;

    virtual void move(const Coord to) = 0;

    virtual void redraw() = 0;

    virtual Coord coord() const = 0;

    virtual void upDown(const int count) = 0;

    virtual void leftRight(const int count) = 0;

protected:
    Cursor() = default;

private:
    Cursor(const Cursor&) = delete;

    Cursor& operator=(const Cursor&) = delete;
};

} // namespace sle

#endif // SLE_CURSOR_H
