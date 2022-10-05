#ifndef SLE_SIDEBAR_H
#define SLE_SIDEBAR_H

#include "sle/context.h"
#include "sle/screen.h"
#include "sle/buffer.h"
#include <memory>

namespace sle {

class Context;
class Buffer;
class ScreenManager;

class SideBar;
using SideBarPtr = std::unique_ptr<SideBar>;


class SideBar {
public:
    static SideBarPtr create(const Context* context);

    virtual ~SideBar() = default;

    virtual void refresh() = 0;

protected:
    SideBar() = default;

private:
    SideBar(const SideBar&) = delete;

    SideBar& operator=(const SideBar&) = delete;
};

} // namespace sle

#endif // SLE_SIDEBAR_H
