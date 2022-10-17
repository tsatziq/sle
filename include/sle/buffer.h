#ifndef SLE_BUFFER_H
#define SLE_BUFFER_H

#include "sle/context.h"
#include "sle/screen.h"
#include "sle/types.h"
#include <map>
#include <memory>

namespace sle {

enum class BufferId : unsigned;

inline BufferId operator++(BufferId& id)
{
    id = static_cast<BufferId>(static_cast<unsigned>(id) + 1);
    return id;
}

class Context;
class SideBar;
class ScreenManager;

class Buffer;
using BufferPtr = std::unique_ptr<Buffer>;

class Buffer
{
public:
    static BufferPtr create(const Context* context);

    virtual ~Buffer() = default;

    virtual void readFile(const std::string& path) = 0;

    virtual void saveFile(const std::string& path) = 0;

    virtual int getSize() const = 0;

    virtual const Text* getData() const = 0;

    virtual void clear() = 0;

    virtual void addLines(const Text& strs) = 0;

    virtual void addChar(const char c) = 0;

    virtual bool eraseChar(const int num) = 0;

    /// Erase line, return true if cursor y pos must be adjusted.
    virtual bool eraseLine(const int num) = 0;

protected:
    Buffer() = default;

private:
    Buffer(const Buffer&) = delete;

    Buffer& operator=(const Buffer&) = delete;
};

} // namespace sle

#endif // SLE_BUFFER_H
