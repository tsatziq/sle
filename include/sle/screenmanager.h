#ifndef SLE_SCREENMANAGER_H
#define SLE_SCREENMANAGER_H

#include "sle/screenmanager.h"
#include "sle/coord.h"
#include "sle/screen.h"
#include <memory>

namespace sle {

class Context;
class Buffer;
class SideBar;

class ScreenManager;
using ScreenManagerPtr = std::unique_ptr<ScreenManager>;

class ScreenManager {
public:
    static ScreenManagerPtr create(const Context* context);

    virtual ~ScreenManager() = default;

    virtual ScreenId addScreen(const Coord& startPoint) = 0;

    virtual void deleteScreen(const ScreenId screen) = 0;

    virtual Screen* getScreen(const ScreenId id) const = 0;

protected:
    ScreenManager() = default;

private:
    ScreenManager(const ScreenManager&) = delete;

    ScreenManager& operator=(const ScreenManager&) = delete;
};

} // namespace sle

#endif // SLE_SCREENMANAGER_H
