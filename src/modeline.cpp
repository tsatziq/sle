#include "sle/modeline.h"
#include "sle/eventaction.h"
#include "sle/screenmanager.h"
#include <stdio.h>

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

    void paint(const char c) override;

    char getChar() const override;

    std::string getLine() const override;

    CursePtr getCurse() const override;


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

	char testFile[] = "test.cpp";
	int length = width_ - (sizeof(testFile) + 2);
	std::string padding(length, '.');
    char modelineStr[width_];
	sprintf(modelineStr, "[%s]%s", testFile, padding.c_str());
    paint({std::string(modelineStr)});
	
	wrefresh(screen_);
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
   for (std::string s : text)
        wprintw(screen_, s.c_str());
}

void ModeLineImpl::paint(const char c)
{}

char ModeLineImpl::getChar() const
{
    return 'x';
}

std::string ModeLineImpl::getLine() const
{
    return "";
}

CursePtr ModeLineImpl::getCurse() const
{
    return screen_;
}

}
