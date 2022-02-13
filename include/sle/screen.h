#ifndef SLE_SCREEN_H
#define SLE_SCREEN_H

#include <string>
#include <vector>
#include <memory>
#include <ncurses.h>

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

struct StrPacket {
    std::vector<std::string> lines;
};

enum class ScreenId : unsigned;

using CursePtr = WINDOW*;

class ScreenBase
{
public:
    virtual ~ScreenBase() = default;

    virtual void paint(
        const StrPacket& text) = 0;

protected:
    ScreenBase() = default;

private:
    ScreenBase(const ScreenBase&) = delete;

    ScreenBase& operator=(const ScreenBase&) = delete;
};

using ScreenPtr = std::shared_ptr<ScreenBase>;

} // namespace sle


#endif // SLE_SCREEN_H
