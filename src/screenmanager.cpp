#include "sle/screenmanager.h"
#include "sle/coord.h"
#include "sle/context.h"
#include "sle/screen.h"
#include <map>
#include <ncurses.h>

namespace sle {

class ScreenManagerImpl : public ScreenManager
{
public:
    static std::unique_ptr<ScreenManagerImpl> create(const Context* context)
    {
        return std::unique_ptr<ScreenManagerImpl>(new ScreenManagerImpl(context));
    }

    ~ScreenManagerImpl();

    ScreenId addScreen(const Coord& startPoint) override;

    void deleteScreen(const ScreenId screen) override;

    Screen* screen(const ScreenId id) const override;

private:
    ScreenManagerImpl(const Context* context);

    const Context* c_;
    ScreenId prevId_;
    std::map<ScreenId, ScreenPtr> screens_;

    int maxWidth_, maxHeight_;
    CursePtr initScr_;

    static constexpr int sidebarWidth{8};
    static constexpr int cmdHeight{1};
    static constexpr int modelineHeight{1};
};

ScreenManagerImpl::ScreenManagerImpl(const Context* context)
    : c_(context), prevId_(ScreenId::mode)
{
    initScr_ = initscr();
    noecho();
    cbreak();

    getmaxyx(initScr_, maxHeight_, maxWidth_);

    // Create the basic screen structure.
    int h, w, y, x;

    h = maxHeight_ - cmdHeight - modelineHeight;
    w = maxWidth_ - sidebarWidth;
    x = sidebarWidth;
    y = 0;
    screens_.emplace(ScreenId::main, Screen::create(h, w, {x, y}));

    h = maxHeight_ - modelineHeight - 1;
    w = sidebarWidth - 1;
    x = 0;
    y = 0;
    screens_.emplace(ScreenId::side, Screen::create(h, w, {x, y}));

    h = cmdHeight;
    w = maxWidth_ - sidebarWidth;
    x = sidebarWidth;
    y = maxHeight_ - cmdHeight - modelineHeight;
    screens_.emplace(ScreenId::cmd, Screen::create(h, w, {x, y}));

    h = modelineHeight;
    w = maxWidth_;
    x = 0;
    y = maxHeight_ - modelineHeight;
    screens_.emplace(ScreenId::mode, Screen::create(h, w, {x, y}));

    refresh();
};

ScreenManagerPtr ScreenManager::create(const Context* context)
{
    return ScreenManagerImpl::create(context);
}

ScreenManagerImpl::~ScreenManagerImpl()
{
    endwin();
}

ScreenId ScreenManagerImpl::addScreen(const Coord& startPoint)
{
    return ScreenId(0);
}

void ScreenManagerImpl::deleteScreen(const ScreenId screen)
{}

Screen* ScreenManagerImpl::screen(const ScreenId id) const
{
    Screen* scr{nullptr};

    if (screens_.count(id))
        scr = screens_.at(id).get();

    return scr;
}

} // namespace sle
