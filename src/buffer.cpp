#include "sle/buffer.h"
#include "sle/eventaction.h"
#include "sle/mainscreen.h"

namespace sle {

class Buffer::Impl {
public:
    Impl(const DispatcherPtr& dispatcher, const StrPacket& text);

protected:
    const DispatcherPtr dispatcher_;
    EventActionContainer actions_;
    std::map<unsigned, std::string> lines_;
    unsigned numLines_;
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
    , numLines_(0)
    , modified_(false)
{
    dispatcher_->sendEvent(PaintMainScr({text}));
}

}
