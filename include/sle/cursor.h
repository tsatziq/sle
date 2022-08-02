#ifndef SLE_CURSOR_H
#define SLE_CURSOR_H

#include "sle/screenmanager.h"
#include "sle/types.h"
#include <memory>

namespace sle {

class Cursor;
using CursorPtr = std::unique_ptr<Cursor>;

class Cursor
{
public:
    static CursorPtr create(const ScreenManager* scrs, const Text& txt);

    virtual ~Cursor() = default;

    virtual void move(const Coord to, bool setX, bool setY) = 0;

    virtual void up(const unsigned count) = 0;

    virtual void down(const unsigned count) = 0;

    virtual void back(const unsigned count) = 0;

    virtual void forward(const unsigned count) = 0;

    virtual void backWord(const unsigned num) = 0;

    virtual Coord coord() const = 0;

protected:
    Cursor() = default;

private:
    Cursor(const Cursor&) = delete;

    Cursor& operator=(const Cursor&) = delete;
};

} // namespace sle

#endif // SLE_CURSOR_H
