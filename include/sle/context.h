#ifndef SLE_CONTEXT_H
#define SLE_CONTEXT_H

#include <memory>
#include "sle/screenmanager.h"
#include "sle/buffer.h"
#include "sle/sidebar.h"

namespace sle {

class SideBar;
class Buffer;
class ScreenManager;

struct Context
{
    std::unique_ptr<ScreenManager> screens = nullptr;
    std::unique_ptr<Buffer> buffer = nullptr;
    std::unique_ptr<SideBar> sideBar = nullptr;
};

} // namespace sle

#endif // SLE_CONTEXT_H
