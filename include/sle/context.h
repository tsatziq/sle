#ifndef SLE_CONTEXT_H
#define SLE_CONTEXT_H

#include <memory>
#include "sle/screenmanager.h"
#include "sle/buffer.h"
#include "sle/cursor.h"
#include "sle/finder.h"
#include "sle/sidebar.h"

namespace sle {

class SideBar;
class Buffer;
class Cursor;
class Finder;
class ScreenManager;

struct Context
{
    std::unique_ptr<ScreenManager> screens = nullptr;
    std::unique_ptr<Buffer> buffer = nullptr;
    std::unique_ptr<SideBar> sideBar = nullptr;
    std::unique_ptr<Cursor> cursor = nullptr;
    std::unique_ptr<Finder> finder = nullptr;
};

} // namespace sle

#endif // SLE_CONTEXT_H
