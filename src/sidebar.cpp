#include "sle/sidebar.h"
#include "sle/eventaction.h"
#include "sle/screenmanager.h"

#include <iostream>

namespace sle
{

class SideBarImpl :
	public SideBar
{
public:
	static ScreenPtr create(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize)
	{
		return std::shared_ptr<SideBarImpl>(
			new SideBarImpl(dispatcher, maxSize));
	}
	
	~SideBarImpl();
	
	void paint(
        const StrPacket& text) override;

private:
	SideBarImpl(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize);
	
	EventActionContainer actions_;
	CursePtr screen_;
    CursePtr border_;
    CursePtr cmdLabel_;
	int height_;
};

SideBarImpl::SideBarImpl(
	const DispatcherPtr& dispatcher,
	const ScreenSize& maxSize) :
actions_(dispatcher)
{
	height_ = maxSize.height - MODELINE_HEIGHT - 1;
	screen_ = newwin(height_, SIDEBAR_WIDTH - 1, 0, 0);
    border_ = newwin(height_ + 1, 1, 0, SIDEBAR_WIDTH - 1);
    cmdLabel_ = newwin(1, SIDEBAR_WIDTH - 1, height_, 0);
	
    for (unsigned i = 0; i < height_ + 1; ++i) {
        mvwaddch(border_, i, 0, '|');  
    }

    wprintw(cmdLabel_, "cmd>");

	wrefresh(screen_);
    wrefresh(cmdLabel_);
    wrefresh(border_);

    actions_.on<ScreenSizeChanged>([&](const ScreenSizeChanged& data)
    {
        height_ = data.size.height - MODELINE_HEIGHT - 1;
       // resize/init screens? 
    });

    actions_.on<AddToSideBar>([&](const AddToSideBar& data)
    {
        for (std::string str : data.lines) {
            str.append("\n");
            wprintw(screen_, str.c_str());
        }
        wrefresh(screen_);
    });
    
    actions_.on<RefreshScreens>([&](const RefreshScreens&)
    {
        //wrefresh(screen_);
        //wrefresh(cmdLabel_);
        //wrefresh(border_);
    });
}

ScreenPtr SideBar::create(
	const DispatcherPtr& dispatcher,
	const ScreenSize& maxSize)
{
	return SideBarImpl::create(dispatcher, maxSize);
}

SideBarImpl::~SideBarImpl()
{
	delwin(screen_);
    delwin(border_);
    delwin(cmdLabel_);
}

void SideBarImpl::paint(
	const StrPacket& text)
{}

}
