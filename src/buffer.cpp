#include "sle/buffer.h"
#include "sle/eventaction.h"
#include "sle/mainscreen.h"
#include "sle/linetype.h"
#include "sle/screenmanager.h"
#include "sle/sidebar.h"

namespace sle {

class Buffer::Impl {
public:
    Impl(const DispatcherPtr& dispatcher, const StrPacket& text);

private:
    void addToScreen(const LineNum from);

    const DispatcherPtr dispatcher_;
    EventActionContainer actions_;
    std::map<LineNum, std::string> lines_;
    LineNum topVisibleLine_;
    LineNum screenHeight_;
    bool modified_;
};

Buffer::Buffer(const DispatcherPtr& dispatcher, const StrPacket& text)
    : impl_(new Impl(dispatcher, text))
{}

Buffer::~Buffer()
{}

Buffer::Impl::Impl(const DispatcherPtr& dispatcher, 
    const StrPacket& text)
    : dispatcher_(dispatcher)
    , actions_(dispatcher)
    , topVisibleLine_(LineNum(1))
    , screenHeight_(LineNum(0))
    , modified_(false)
{
    LineNum n = LineNum(1);

    for (std::string line : text) {
        lines_.insert({n, line});
        ++n;
    }
        
    actions_.on<ScreenSizeChanged>([&](const ScreenSizeChanged& data)
    {
        screenHeight_ = LineNum(data.size.height);
    });

    actions_.on<ScreensReady>([&](const ScreensReady&)
    {
        addToScreen(topVisibleLine_);
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
