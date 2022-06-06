#ifndef SLE_TYPES_H
#define SLE_TYPES_H

#include <ncurses.h>
#include <string>
#include <vector>

namespace sle {

struct Coord {
    int x;
    int y;
};
 
using CursePtr = WINDOW*;

using Text = std::vector<std::string>;

} // namespace sle 

#endif // SLE_TYPES_H
