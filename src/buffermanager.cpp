#include "sle/buffermanager.h"
#include <map>

namespace sle {

class BufferManagerImpl :
    public BufferManager
{
public:
    static std::shared_ptr<BufferManagerImpl> create(
        const DispatcherPtr& dispatcher)
    {
        return std::shared_ptr<BufferManagerImpl>(new BufferManagerImpl(
            dispatcher));
    };

    ~BufferManagerImpl();

    BufferId addBuffer() override;

    void deleteBuffer(BufferId id) override;

    BufferPtr getBuffer(BufferId id) const override;

private:
    BufferManagerImpl(const DispatcherPtr& dispatcher);

    const DispatcherPtr dispatcher_;
    BufferId prevId_;
    std::map<BufferId, BufferPtr> bufs_;
    BufferPtr bufp_;
};

BufferManagerImpl::BufferManagerImpl(const DispatcherPtr& dispatcher)
    : prevId_(BufferId(0))
{}

BufferManagerPtr BufferManager::create(const DispatcherPtr& dispatcher)
{
    return BufferManagerImpl::create(dispatcher);
}

BufferManagerImpl::~BufferManagerImpl()
{}

BufferId BufferManagerImpl::addBuffer()
{
    BufferId id = ++prevId_;
    //BufferPtr buf = std::make_shared<Buffer>(dispatcher_, id);
    //bufs_.insert({id, std::move(buf)});
    bufs_.insert({id, std::make_shared<Buffer>(dispatcher_, id)});
    //bufp_ = std::make_shared<Buffer>(dispatcher_, id);
    return id;
}

void BufferManagerImpl::deleteBuffer(BufferId id)
{}

BufferPtr BufferManagerImpl::getBuffer(BufferId id) const
{
    return nullptr;
}

}
