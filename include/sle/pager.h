#ifndef SLE_PAGER_H
#define SLE_PAGER_H

#include "sle/types.h"
#include <memory>

namespace sle {

class Context;
class Buffer;
class ScreenManager;

class Pager;
using PagerPtr = std::unique_ptr<Pager>;

class Pager
{
public:
    static PagerPtr create(const Context* context);

    virtual ~Pager() = default;

    virtual void show() = 0;

    virtual void movePage(const int count) = 0;

    virtual int lastLineNum() = 0;

    virtual int firstLineNum() = 0;

    virtual int pageNum() = 0;

    virtual int numLinesOnScr() = 0;

protected:
    Pager() = default;

private:
    Pager(const Pager&) = delete;

    Pager& operator=(const Pager&) = delete;
};

} // namespace sle

#endif // SLE_PAGER_H
