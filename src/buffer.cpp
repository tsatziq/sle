#include "sle/buffer.h"
#include "sle/eventaction.h"
#include "sle/mainscreen.h"

namespace sle {

class Buffer::Impl {
public:
    Impl(const DispatcherPtr& dispatcher, std::vector<std::string> txt);

protected:
    const DispatcherPtr dispatcher_;
    EventActionContainer actions_;
    std::map<char[7], std::string> lines_;
    unsigned numLines_;
    bool modified_;
};

Buffer::Buffer(const DispatcherPtr& dispatcher, std::vector<std::string> txt)
    : impl_(new Impl(dispatcher, txt))
{}

Buffer::~Buffer()
{}

Buffer::Impl::Impl(const DispatcherPtr& dispatcher, 
    std::vector<std::string> txt)
    : dispatcher_(dispatcher)
    , actions_(dispatcher)
    , numLines_(0)
    , modified_(false)
{
    dispatcher_->sendEvent(PaintMainScr({txt}));
}

}
