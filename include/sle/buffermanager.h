#ifndef SLE_BUFFERMANAGER_H
#define SLE_BUFFERMANAGER_H

#include "sle/dispatcher.h"
#include "sle/buffer.h"
#include <memory>

namespace sle {

class BufferManager;
using BufferManagerPtr = std::shared_ptr<BufferManager>;

class BufferManager {
public:
    static BufferManagerPtr create(const DispatcherPtr& dispatcher); 

    virtual ~BufferManager() = default;

    virtual BufferId addBuffer() = 0;

    virtual void deleteBuffer(BufferId id) = 0;

    virtual BufferPtr getBuffer(BufferId id) const = 0;

protected:
    BufferManager() = default;
};

} // namespace sle

#endif // SLE_BUFFERMANAGER_H
