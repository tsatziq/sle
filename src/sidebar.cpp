#include "sle/sidebar.h"
#include "sle/eventaction.h"
#include "sle/screenmanager.h"

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
	int height_;
};

SideBarImpl::SideBarImpl(
	const DispatcherPtr& dispatcher,
	const ScreenSize& maxSize) :
actions_(dispatcher)
{
	height_ = maxSize.height - MODELINE_HEIGHT;
	screen_ = newwin(height_, SIDEBAR_WIDTH, 0, 0);
	
	wborder(screen_, ' ', '|', ' ', ' ', ' ', '|', ' ', '|');
	//box(screen_, '|', ' ');
	wrefresh(screen_);
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
}

void SideBarImpl::paint(
	const StrPacket& text)
{}

}
