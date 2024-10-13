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
class CmdLine;
struct Point;

struct Context
{
    Buffer* buf = nullptr;
    Buffer* prev = nullptr;
    EditLoop* loop = nullptr;
    FileHandler* file = nullptr;
    MainScreen* scr = nullptr;
    CmdLine* cln = nullptr;

    RangePtr visibleRange = Range::make(
        Point::make(0, 0, false, true),
        Point::make(0, 0, false, true));
};

} // namespace sle

#endif // SLE_CONTEXT_H
