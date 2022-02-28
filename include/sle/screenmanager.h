#ifndef SLE_SCREENMANAGER_H
#define SLE_SCREENMANAGER_H

#include "sle/dispatcher.h"
#include "sle/screen.h"
#include <memory>

namespace sle
{

enum class ScreenID2 : unsigned {
    main = 1,
    sidebar = 2,
    cmdline = 3,
    modeline = 4,
};

enum : int
{
	SIDEBAR_WIDTH = 7,
	CMDLINE_INIT_HEIGHT = 1,
	MODELINE_HEIGHT = 1,
};

struct ScreenSizeChanged {
    const ScreenSize size;
};

struct ScreensReady {};

class ScreenManager;
using ScreenManagerPtr = std::shared_ptr<ScreenManager>;

class ScreenManager
{
public:
    static ScreenManagerPtr create(
        const DispatcherPtr& dispatcher);

    virtual ~ScreenManager() = default;

    virtual ScreenId addScreen(
        const Coord& startPoint) = 0;

    virtual void deleteScreen(
        const ScreenId screen) = 0;

protected:
    ScreenManager() = default;
};

} // namespace sle

#endif // SLE_SCREENMANAGER_H
