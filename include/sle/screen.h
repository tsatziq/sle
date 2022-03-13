#ifndef SLE_SCREEN_H
#define SLE_SCREEN_H

#include <string>
#include <vector>
#include <memory>
#include <ncurses.h>
#include "sle/linetype.h"

namespace sle
{

struct Coord
{
    int x;
    int y;

    Coord(
        const int xIn,
        const int yIn) :
    x(xIn),
    y(yIn)
    {};
};

struct ScreenSize
{
    int height;
    int width;

    ScreenSize(
        int heightIn,
        int widthIn) :
    height(heightIn),
    width(widthIn)
    {};

    // Add operators for plus minus etc!
};

enum class ScreenId : unsigned {
    main = 1,
    sidebar = 2,
    cmdline = 3,
    modeline = 4
};

inline ScreenId operator++(ScreenId& id)
{
    return static_cast<ScreenId>(static_cast<unsigned>(id) + 1);
}

using CursePtr = WINDOW*;

class ScreenBase
{
public:
    virtual ~ScreenBase() = default;

    virtual void paint(
        const StrPacket& text) = 0;

    virtual void paint(const char c) = 0;

    virtual char getChar() const = 0;

    virtual std::string getLine() const = 0;

    virtual CursePtr getCurse() const = 0;

protected:
    ScreenBase() = default;

private:
    ScreenBase(const ScreenBase&) = delete;

    ScreenBase& operator=(const ScreenBase&) = delete;
};

using ScreenPtr = std::shared_ptr<ScreenBase>;

} // namespace sle


#endif // SLE_SCREEN_H
