#include "sle/buffer.h"
#include "sle/eventaction.h"
#include "sle/filehandler.h"
#include "sle/mainscreen.h"
#include "sle/linetype.h"
#include "sle/screenmanager.h"
#include "sle/sidebar.h"

#include <iostream>

namespace sle {

class Buffer::Impl {
public:
    Impl(const DispatcherPtr& dispatcher,
        std::string path,
        const ScreenPtr& screen);

private:
    void addToScreen(const LineNum from);

    const DispatcherPtr dispatcher_;
    EventActionContainer actions_;
    const FileHandlerPtr fileHandler_;
    const ScreenPtr screen_;

    std::map<LineNum, std::string> lines_;
    LineNum screenHeight_;
    LineNum topVisibleLine_;
    bool modified_;
};

Buffer::Buffer(const DispatcherPtr& dispatcher,
    std::string path,
    const ScreenPtr& screen)
    : impl_(new Impl(dispatcher, path, screen))
{}

Buffer::~Buffer()
{}

Buffer::Impl::Impl(const DispatcherPtr& dispatcher,
	std::string path,
	const ScreenPtr& screen)
	: dispatcher_(dispatcher)
	, actions_(dispatcher)
    , screen_(screen)
    , fileHandler_(FileHandler::create(path))
    , topVisibleLine_(LineNum(1))
    , screenHeight_(LineNum(0))
    , modified_(false)
{
    StrPacket text = fileHandler_->getContents();
    
    LineNum n = LineNum(1);

    for (std::string line : text) {
        lines_.insert({n, line});
        ++n;
    }

	addToScreen(topVisibleLine_);
        
    actions_.on<ScreenSizeChanged>([&](const ScreenSizeChanged& data)
    {
        screenHeight_ = LineNum(data.size.height);
    });  
}

void Buffer::Impl::addToScreen(const LineNum from)
{
    StrPacket text;
    StrPacket linenums;

    for (auto pair : lines_) {
        if (pair.first < screenHeight_)
            text.push_back(pair.second);
    }

    LineNum size(static_cast<int>(text.size()));

    for (LineNum n = LineNum(1); n < (size + 1); ++n)
		linenums.push_back(n.toStr());
    
    dispatcher_->sendEvent(PaintMainScr({text}));
    dispatcher_->sendEvent(AddToSideBar({linenums}));
}

}
