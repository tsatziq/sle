#include "sle/screenmanager.h"
#include "sle/eventaction.h"
#include "sle/mainscreen.h"
#include "sle/sidebar.h"
#include "sle/cmdline.h"
#include "sle/modeline.h"
#include <ncurses.h>

namespace sle
{

class ScreenManagerImpl :
    public ScreenManager
{
public:
    static std::shared_ptr<ScreenManagerImpl> create(
        const DispatcherPtr& dispatcher)
    {
        return std::shared_ptr<ScreenManagerImpl>(
            new ScreenManagerImpl(dispatcher));
    };

    ~ScreenManagerImpl();

    ScreenId addScreen(
        const Coord& startPoint) override;

    void deleteScreen(
        const ScreenId screen) override;

    ScreenPtr getScreen(const ScreenId id) const override;

private:
    ScreenManagerImpl(
        const DispatcherPtr& dispatcher);

    const DispatcherPtr dispatcher_;
    EventActionContainer actions_;
    ScreenId prevId_;
    std::map<ScreenId, ScreenPtr> screens_;
    
    int maxWidth_, maxHeight_;
	CursePtr initScr_;
};

ScreenManagerImpl::ScreenManagerImpl(
    const DispatcherPtr& dispatcher) :
dispatcher_(dispatcher),
actions_(dispatcher),
prevId_(ScreenId::modeline)
{
	initScr_ = initscr();
	cbreak();
	
    getmaxyx(initScr_, maxHeight_, maxWidth_);
    dispatcher_->sendEvent(ScreenSizeChanged({ScreenSize(maxHeight_, maxWidth_)}));

	ScreenPtr mainScr = MainScreen::create(
	    dispatcher_,
	    ScreenSize(maxHeight_, maxWidth_));
    screens_.emplace(ScreenId::main, mainScr);

	ScreenPtr sideBar = SideBar::create(
		dispatcher_,
		ScreenSize(maxHeight_, maxWidth_));
    screens_.emplace(ScreenId::sidebar, sideBar);
		
	ScreenPtr cmdLine = CmdLine::create(
		dispatcher_,
		ScreenSize(maxHeight_, maxWidth_));
    screens_.emplace(ScreenId::cmdline, cmdLine);
		
	ScreenPtr modeLine = ModeLine::create(
		dispatcher_,
		ScreenSize(maxHeight_, maxWidth_));
    screens_.emplace(ScreenId::modeline, modeLine);

    dispatcher_->sendEvent(ScreensReady());
};

ScreenManagerPtr ScreenManager::create(
	const DispatcherPtr& dispatcher)
{
	return ScreenManagerImpl::create(dispatcher);
}

ScreenManagerImpl::~ScreenManagerImpl()
{
	endwin();
}

ScreenId ScreenManagerImpl::addScreen(
	const Coord& startPoint)
{
	return ScreenId(0);
}

void ScreenManagerImpl::deleteScreen(
	const ScreenId screen)
{}

ScreenPtr ScreenManagerImpl::getScreen(const ScreenId id) const
{
    try {
        return screens_.at(id);
    }
    catch (const std::out_of_range& e) {
        // Make a cmdLine event or a fault maybe that
        // displays messaged for couple of seconds and clears it
        return nullptr;
    }
}


} // namespace sle
