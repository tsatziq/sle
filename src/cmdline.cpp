#include "sle/cmdline.h"
#include "sle/eventaction.h"
#include "sle/screenmanager.h"

namespace sle
{

class CmdLineImpl :
	public CmdLine
{
public:
	static ScreenPtr create(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize)
	{
		return std::shared_ptr<CmdLineImpl>(
			new CmdLineImpl(dispatcher, maxSize));
	}
	
	~CmdLineImpl();
	
	void paint(
        const StrPacket& text) override;

    void paint(const char c) override;

    char getChar() const override;

    std::string getLine() const override;

    CursePtr getCurse() const override;

private:
	CmdLineImpl(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize);
	
	EventActionContainer actions_;
	CursePtr screen_;
	int height_, width_;
	Coord startPoint_;
};

CmdLineImpl::CmdLineImpl(
	const DispatcherPtr& dispatcher,
	const ScreenSize& maxSize) :
actions_(dispatcher),
height_(CMDLINE_INIT_HEIGHT),
startPoint_(Coord(maxSize.height - CMDLINE_INIT_HEIGHT - MODELINE_HEIGHT,
	SIDEBAR_WIDTH))
{
	width_ = maxSize.width - SIDEBAR_WIDTH;
	screen_ = newwin(height_, width_, startPoint_.x, startPoint_.y);
	
	wrefresh(screen_);
}

ScreenPtr CmdLine::create(
	const DispatcherPtr& dispatcher,
	const ScreenSize& maxSize)
{
	return CmdLineImpl::create(dispatcher, maxSize);
}

CmdLineImpl::~CmdLineImpl()
{
	delwin(screen_);
}

void CmdLineImpl::paint(
	const StrPacket& text)
{}

void CmdLineImpl::paint(const char c)
{}

char CmdLineImpl::getChar() const
{
    return 'x';
}

std::string CmdLineImpl::getLine() const
{
    return "";
}

CursePtr CmdLineImpl::getCurse() const
{
    return screen_;
}

}
