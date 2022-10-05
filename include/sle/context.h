#ifndef SLE_CONTEXT_H
#define SLE_CONTEXT_H

#include "sle/screenmanager.h"
#include "sle/buffer.h"
#include "sle/cursor.h"
#include "sle/sidebar.h"
#include <memory>

namespace sle {

class Context;
using ContextPtr = std::shared_ptr<Context>;

class Context
{
public:
    Context()
        : screens(ScreenManager::create())
        , buffer(Buffer::create(screens->getScreen(ScreenId::main)))
        , sideBar(SideBar::create(
            screens->getScreen(ScreenId::side),
            screens->getScreen(ScreenId::main),
            buffer.get()))
        , cursor(Cursor::create(screens.get(), buffer->getData()))
    {}

    ScreenManagerPtr screens;
    BufferPtr buffer;
    SideBarPtr sideBar;
    CursorPtr cursor;
};

} // namespace sle

#endif // SLE_CONTEXT_H
