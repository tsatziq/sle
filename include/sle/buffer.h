#ifndef SLE_BUFFER_H
#define SLE_BUFFER_H

#include "sle/dispatcher.h"
#include "sle/screen.h"
#include <map>
#include <memory>

namespace sle {

enum class BufferId : unsigned;

inline BufferId operator++(BufferId& id)
{
    return static_cast<BufferId>(static_cast<unsigned>(id) + 1);
}

struct ReadFile {
    const BufferId id;
    const std::string path;
};

struct SaveFile {
    const BufferId id;
    const std::string path;
};

struct ShowBuffer {
    const BufferId id;
    const ScreenPtr screen;
};

class Buffer;
using BufferPtr = std::shared_ptr<Buffer>;

class Buffer {
public:
    Buffer(const DispatcherPtr& dispatcher, BufferId id);

    ~Buffer();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace sle

#endif // SLE_BUFFER_H
