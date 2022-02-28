#include "sle/cmdline.h"
#include "sle/eventaction.h"
#include "sle/screenmanager.h"
#include "sle/buffer.h"
#include <string>

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

private:
	CmdLineImpl(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize);
		
	void getInput();
	
	int parseInput(std::string& input);

    int interpretRange(std::string str);
	
	EventActionContainer actions_;
	const DispatcherPtr& dispatcher_;
	CursePtr screen_;
	int height_, width_;
	Coord startPoint_;
    const std::string cmds = "ai";
};

CmdLineImpl::CmdLineImpl(
	const DispatcherPtr& dispatcher,
	const ScreenSize& maxSize) :
actions_(dispatcher),
dispatcher_(dispatcher),
height_(CMDLINE_INIT_HEIGHT),
startPoint_(Coord(maxSize.height - CMDLINE_INIT_HEIGHT - MODELINE_HEIGHT,
	SIDEBAR_WIDTH))
{
	width_ = maxSize.width - SIDEBAR_WIDTH;
	screen_ = newwin(height_, width_, startPoint_.x, startPoint_.y);
	
	wrefresh(screen_);
	
	actions_.on<RefreshScreens>([&](const RefreshScreens&)
    {
        //wrefresh(screen_);
    });

/*	
	actions_.on<BufferReady>([&](const BufferReady&)
    {
        getInput();
    });
*/
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

void CmdLineImpl::getInput()
{
    std::string cmd;
    int c;
    int returnVal;

  /* 
    do { 
        cmd.clear();

		while ((c = wgetch(screen_)) != '\n') {
			switch (c) {
			case 127:
				if (!cmd.empty()) {
					cmd.pop_back();
					werase(screen_);
					wprintw(screen_, cmd.c_str());
				}
				break;
				
			case 0x03:
				werase(screen_);
				cmd.clear();
				break;
				
			default:
				waddch(screen_, c);
				//wrefresh(screen_);
				cmd += c;
				break;
			}
		}

        werase(screen_);
        //wrefresh(screen_);
    } while ((returnVal = parseInput(cmd)) == 0);

    if (returnVal == 1)
        dispatcher_->stopDispatching();
*/
}

int CmdLineImpl::parseInput(std::string& input)
{
    int ret = 0;
    //make a struct with the below or maybe not, event will be struct anyway
    std::string text, cmd, ln1, ln2, extra;

    if (input == "q")
        ret = 1;

    size_t pos = input.find(':');
    if (pos != std::string::npos) {
       text = input.substr(pos);
       text.erase(text.begin());
       input.erase(pos); 
    }

    pos = input.find(',');
    if (pos == std::string::npos) {
        if (cmds.find(input.back())) {
            interpretRange(input);
        }
            // try to interpret rest as a line? maybe make another func
    } 

    return ret;
}

int CmdLineImpl::interpretRange(std::string str)
{
    std::string range = str;
    str.pop_back();

    

    return 1;
}
// aai/moi:hei
// aai
// aai:hei

/*
    -muista komento voi olla vaan: ab (eli siirry riville)! 
        -toi pitaa eka varmistaa
        -sillon ei ole pilkkua, ehka auttaa
    -oliko sita mitaan komentoja missa on kahta kirjainta jos on vaan rivinumero ja jotain?
    -syntaksin muutos nayttaa vaan houkuttelevammalta koko ajan
*/

}
