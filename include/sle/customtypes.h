#ifndef SLE_CUSTOMTYPES_H
#define SLE_CUSTOMTYPES_H

#include <cctype>
#include <functional>

namespace sle
{

enum class Direction
{
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

// NOTE: jos nait tulee paljo, erota joku ctype oma.
static constexpr int(*const isWSpace)(int) = std::isspace;
static constexpr int(*isChar)(int) = std::isgraph;
enum class CType
{
    NONE,
    WSPACE,
    NONWS,
};

} // namespace sle

#endif // SLE_CUSTOMTYPES_H
