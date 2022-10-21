#ifndef SLE_FINDER_H
#define SLE_FINDER_H

#include "sle/coord.h"
#include "sle/types.h"
#include <memory>

namespace sle {

class Context;
class Buffer;
class ScreenManager;

class Finder;
using FinderPtr = std::unique_ptr<Finder>;

class Finder
{
public:
    static FinderPtr create(const Context* context);

    virtual ~Finder() = default;

    virtual Coord findWord() = 0;

protected:
    Finder() = default;

private:
    Finder(const Finder&) = delete;

    Finder& operator=(const Finder&) = delete;
};

} // namespace sle

#endif // SLE_FINDER_H
