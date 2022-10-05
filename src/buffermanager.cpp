#include "sle/buffermanager.h"
#include <map>

namespace sle {

class BufferManagerImpl : public BufferManager
{
public:
    static std::unique_ptr<BufferManagerImpl> create()
    {
        return std::unique_ptr<BufferManagerImpl>(new BufferManagerImpl());
    };

    ~BufferManagerImpl();

    BufferId addBuffer() override;

    void deleteBuffer(BufferId id) override;

    Buffer* getBuffer(BufferId id) const override;

private:
    BufferManagerImpl();

    BufferId prevId_;
    std::map<BufferId, BufferPtr> bufs_;
};

BufferManagerImpl::BufferManagerImpl()
    : prevId_(BufferId(0))
{}

BufferManagerPtr BufferManager::create()
{
    return BufferManagerImpl::create();
}

BufferManagerImpl::~BufferManagerImpl()
{}

BufferId BufferManagerImpl::addBuffer()
{
    bufs_.emplace(++prevId_, Buffer::create(nullptr));
    return BufferId(prevId_);
}

void BufferManagerImpl::deleteBuffer(BufferId id)
{}

Buffer* BufferManagerImpl::getBuffer(BufferId id) const
{
    return bufs_.at(id).get(); 
}

}
