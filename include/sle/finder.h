#ifndef SLE_FINDER_H
#define SLE_FINDER_H

#include "sle/types.h"
#include <memory>

namespace sle {

class Finder;
using FinderPtr = std::unique_ptr<Finder>;

class Finder
{
public:
    static FinderPtr create(const Text* txt);

    virtual ~Finder() = default;

protected:
    Finder() = default;

private:
    Finder(const Finder&) = delete;

    Finder& operator=(const Finder&) = delete;
};

} // namespace sle

#endif // SLE_FINDER_H
