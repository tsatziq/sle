#include "sle/buffer.h"
#include "sle/eventaction.h"
#include "sle/filehandler.h"
#include "sle/linetype.h"
#include "sle/screenmanager.h"

namespace sle {

class Buffer::Impl {
public:
    Impl(const DispatcherPtr& dispatcher, BufferId id);

private:
    void addToScreen(const LineNum from);

    const DispatcherPtr dispatcher_;
    const BufferId id_;
    EventActionContainer actions_;
    const FileHandlerPtr fileHandler_;
  
    std::map<LineNum, std::string> lines_;
    LineNum topVisibleLine_;
    LineNum screenHeight_;
    bool modified_;
};

Buffer::Buffer(const DispatcherPtr& dispatcher, BufferId id)
    : impl_(new Impl(dispatcher, id))
{}

Buffer::~Buffer()
{}

Buffer::Impl::Impl(const DispatcherPtr& dispatcher, BufferId id)
    : dispatcher_(dispatcher)
    , id_(id)
    , actions_(dispatcher)
    , fileHandler_(FileHandler::create())
    , topVisibleLine_(LineNum(1))
    , screenHeight_(LineNum(0))
    , modified_(false)
{
    actions_.on<ScreenSizeChanged>([&](const ScreenSizeChanged& data)
    {
        screenHeight_ = LineNum(data.size.height);
    });

/*
    actions_.on<ReadFile>([&](const ReadFile& data)
    {
        if (data.id != id_)
            return;

        StrPacket text = fileHandler_->getContents(data.path);

		LineNum n = LineNum(1);

		for (std::string line : text) {
			lines_.insert({n, line});
			++n;
		}
	});
*/
}

void Buffer::Impl::addToScreen(const LineNum from)
{
/*
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
*/
}

}
