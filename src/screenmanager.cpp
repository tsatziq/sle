#include "sle/screenmanager.h"
#include "sle/eventaction.h"
#include "sle/mainscreen.h"
#include "sle/sidebar.h"
#include "sle/cmdline.h"
#include "sle/modeline.h"
#include <ncurses.h>

#include "sle/buffer.h"

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

ScreenId operator++(ScreenId& screen)
{
    screen = static_cast<ScreenId>(static_cast<unsigned>(screen) + 1);
    return screen;
}

ScreenManagerImpl::ScreenManagerImpl(
    const DispatcherPtr& dispatcher) :
dispatcher_(dispatcher),
actions_(dispatcher),
prevId_(ScreenId(0))
{
	initScr_ = initscr();
	noecho();
    cbreak();
    curs_set(0);
    //raw();
	keypad(initScr_, true);
	
    getmaxyx(initScr_, maxHeight_, maxWidth_);
    dispatcher_->sendEvent(ScreenSizeChanged({ScreenSize(maxHeight_, maxWidth_)}));

	ScreenPtr mainScr = MainScreen::create(
	    dispatcher_,
	    ScreenSize(maxHeight_, maxWidth_));
    screens_.emplace(++prevId_, mainScr);

	ScreenPtr sideBar = SideBar::create(
		dispatcher_,
		ScreenSize(maxHeight_, maxWidth_));
    screens_.emplace(++prevId_, sideBar);
		
	ScreenPtr modeLine = ModeLine::create(
		dispatcher_,
		ScreenSize(maxHeight_, maxWidth_));
    screens_.emplace(++prevId_, modeLine);
    
    ScreenPtr cmdLine = CmdLine::create(
		dispatcher_,
		ScreenSize(maxHeight_, maxWidth_));
    screens_.emplace(++prevId_, cmdLine);

    dispatcher_->sendEvent(ScreensReady());
    
    /*
    actions_.on<BufferReady>([&](const BufferReady&)
    {
        int k = getch();
        dispatcher_->sendEvent(RefreshScreens());
    });
    */
    
    actions_.on<RefreshScreens>([&](const RefreshScreens&)
    {
        //refresh();
    });
    
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


} // namespace sle
