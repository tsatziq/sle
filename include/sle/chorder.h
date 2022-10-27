#ifndef SLE_CHORDER_H
#define SLE_CHORDER_H

#include "sle/types.h"
#include <memory>

namespace sle {

class Context;
class Buffer;
class ScreenManager;

class Chorder;
using ChorderPtr = std::unique_ptr<Chorder>;

class Chorder
{
public:
    static ChorderPtr create(const Context* context);

    virtual ~Chorder() = default;

    virtual void tryChord(const char c) = 0;

protected:
    Chorder() = default;

private:
    Chorder(const Chorder&) = delete;
    Chorder& operator=(const Chorder&) = delete;
};

} // namespace sle

#endif // SLE_CHORDER_H
