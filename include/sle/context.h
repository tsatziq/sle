#ifndef SLE_CONTEXT_H
#define SLE_CONTEXT_H

#include "sle/point.h"
#include <memory>

namespace sle
{

class Context;
using ContextPtr = std::shared_ptr<Context>;

class Buffer;
class EditLoop;
class FileHandler;
class MainScreen;
struct Point;

struct Context
{
    Buffer* buf = nullptr;
    EditLoop* loop = nullptr;
    FileHandler* file = nullptr;
    MainScreen* scr = nullptr;

    Range visibleRange;
};

} // namespace sle

#endif // SLE_CONTEXT_H
