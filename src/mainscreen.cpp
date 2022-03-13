#include "sle/mainscreen.h"
#include "sle/eventaction.h"
#include "sle/screenmanager.h"
#include <iostream>

namespace sle
{

class MainScreenImpl :
	public MainScreen
{
public:
	static ScreenPtr create(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize)
	{
		return std::shared_ptr<MainScreenImpl>(
			new MainScreenImpl(dispatcher, maxSize));
	}
	
	~MainScreenImpl();
	
	void paint(
        const StrPacket& text) override;

    void paint(const char c) override;

    char getChar() const override;

    std::string getLine() const override;

    CursePtr getCurse() const override;
        
private:
	MainScreenImpl(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize);
	
	EventActionContainer actions_;
	CursePtr screen_;
	int width_, height_;
};

MainScreenImpl::MainScreenImpl(
	const DispatcherPtr& dispatcher,
	const ScreenSize& maxSize) :
actions_(dispatcher)
{
	height_ = maxSize.height - CMDLINE_INIT_HEIGHT - MODELINE_HEIGHT;
	width_ = maxSize.width - SIDEBAR_WIDTH;
	screen_ = newwin(height_, width_, 0, SIDEBAR_WIDTH);
	
    actions_.on<PaintMainScr>([&](const PaintMainScr& data)
    {
        paint(data.txt);
    });
    
	wrefresh(screen_);
}

ScreenPtr MainScreen::create(
	const DispatcherPtr& dispatcher,
	const ScreenSize& maxSize)
{
	return MainScreenImpl::create(dispatcher, maxSize);
}

MainScreenImpl::~MainScreenImpl()
{
	delwin(screen_);
}

void MainScreenImpl::paint(
	const StrPacket& text)
{
    for (std::string line : text) {
        wprintw(screen_, line.c_str());
    } 

    wrefresh(screen_);
}

void MainScreenImpl::paint(const char c)
{}

char MainScreenImpl::getChar() const
{
    return 'x';
}

std::string MainScreenImpl::getLine() const
{
    return "";
}

CursePtr MainScreenImpl::getCurse() const
{
    return screen_;
}
	
}
