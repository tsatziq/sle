#ifndef SLE_SCREEN_H
#define SLE_SCREEN_H

#include <string>
#include <vector>
#include <memory>
#include <ncurses.h>
#include "sle/coord.h"
#include "sle/types.h"

namespace sle {

enum class ScreenId : unsigned {
    main,
    side,
    mode,
    cmd,
};

inline ScreenId operator++(ScreenId& id) {
    return static_cast<ScreenId>(static_cast<unsigned>(id) + 1);
}

class Screen;
using ScreenPtr = std::unique_ptr<Screen>;

class Screen
{
public:
    static ScreenPtr create(int height, int width, const Coord& start);

    virtual ~Screen() = default;

    virtual void paint(const Text& txt) = 0;

    virtual void paint(const char c) = 0;

    virtual int getChar() const = 0;

    virtual std::string getLine() const = 0;

    virtual CursePtr getCurse() const = 0;

    virtual int getHeight() const = 0;

    virtual int getWidth() const = 0;

protected:
    Screen() = default;

private:
    Screen(const Screen&) = delete;

    Screen& operator=(const Screen&) = delete;
};

} // namespace sle

#endif // SLE_SCREEN_H
