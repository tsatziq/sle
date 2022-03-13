#include "sle/normalmode.h"
#include "sle/eventaction.h"
#include "sle/screenmanager.h"
#include <ncurses.h>

namespace sle {
    class NormalMode::Impl {
public:
    Impl(const DispatcherPtr& dispatcher, const ScreenPtr& scr);

private:
    const DispatcherPtr dispatcher_;
    EventActionContainer actions_;
    const ScreenPtr scr_;
    const CursePtr curse_;
};

NormalMode::NormalMode(const DispatcherPtr& dispatcher, const ScreenPtr& scr)
     : impl_(new Impl(dispatcher, scr))
{}

NormalMode::~NormalMode()
{}

NormalMode::Impl::Impl(const DispatcherPtr& dispatcher, const ScreenPtr& scr)
    : dispatcher_(dispatcher)
    , actions_(dispatcher)
    , scr_(scr)
    , curse_(scr->getCurse())
{
    actions_.on<StartNormalMode>([&](const StartNormalMode&)
    {
        wmove(curse_, 0, 0);
    });
}

}
