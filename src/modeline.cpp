#include "sle/modeline.h"
#include "sle/eventaction.h"
#include "sle/screenmanager.h"

namespace sle
{

class ModeLineImpl :
	public ModeLine
{
public:
	static ScreenPtr create(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize)
	{
		return std::shared_ptr<ModeLineImpl>(
			new ModeLineImpl(dispatcher, maxSize));
	}
	
	~ModeLineImpl();
	
	void paint(
        const StrPacket& text) override;

private:
	ModeLineImpl(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize);
	
	EventActionContainer actions_;
	CursePtr screen_;
	int width_;
	Coord startPoint_;
};

ModeLineImpl::ModeLineImpl(
	const DispatcherPtr& dispatcher,
	const ScreenSize& maxSize) :
actions_(dispatcher),
width_(maxSize.width),
startPoint_(Coord(maxSize.height - MODELINE_HEIGHT, 0))
{
	screen_ = newwin(MODELINE_HEIGHT, width_, startPoint_.x, startPoint_.y);
	paint({});
	
	wrefresh(screen_);
	
	actions_.on<RefreshScreens>([&](const RefreshScreens&)
    {
        //wrefresh(screen_);
    });
}

ScreenPtr ModeLine::create(
	const DispatcherPtr& dispatcher,
	const ScreenSize& maxSize)
{
	return ModeLineImpl::create(dispatcher, maxSize);
}

ModeLineImpl::~ModeLineImpl()
{
	delwin(screen_);
}

void ModeLineImpl::paint(
	const StrPacket& text)
{
	// Muuta myohemmin niin, etta handlet tekee muutokset varibaleen ja paint
	// maalaa VAIKO NIIN etta handlet suoraan muuttaa?
	std::string testFile = "test.cpp";
	int length = width_ - (testFile.length() + 2);
	std::string padding(length, '.');
	wprintw(screen_, "[%s]%s", testFile.c_str(), padding.c_str());
}

}
