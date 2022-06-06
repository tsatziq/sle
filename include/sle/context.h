#ifndef SLE_CONTEXT_H
#define SLE_CONTEXT_H

#include "sle/screenmanager.h"
#include "sle/buffermanager.h"
#include <memory>

namespace sle {

class Context;
using ContextPtr = std::shared_ptr<Context>;

class Context
{
public:
    Context()
        : scrManager(ScreenManager::create())
        , bufManager(BufferManager::create())
    {}

    ScreenManagerPtr scrManager;
    BufferManagerPtr bufManager;
};

} // namespace sle

#endif // SLE_CONTEXT_H
