#ifndef SLE_CONTEXT_H
#define SLE_CONTEXT_H

#include <memory>
#include "sle/screenmanager.h"
#include "sle/buffer.h"
#include "sle/chorder.h"
#include "sle/cursor.h"
#include "sle/finder.h"
#include "sle/pager.h"
#include "sle/sidebar.h"

namespace sle {

class SideBar;
class Buffer;
class Chorder;
class Cursor;
class Finder;
class Pager;
class ScreenManager;

enum class Mode
{
    normal,
    insert,
    cmd,
    quit,
};

struct CurrentMode
{
    Mode mode{};
};

struct Context
{
    std::unique_ptr<ScreenManager> screens = nullptr;
    std::unique_ptr<Buffer> buffer = nullptr;
    std::unique_ptr<SideBar> sideBar = nullptr;
    std::unique_ptr<Cursor> cursor = nullptr;
    std::unique_ptr<Finder> finder = nullptr;
    std::unique_ptr<Pager> pager = nullptr;
    std::unique_ptr<Chorder> chorder = nullptr;
    std::unique_ptr<CurrentMode> currentMode = std::make_unique<CurrentMode>();
};

} // namespace sle

#endif // SLE_CONTEXT_H
