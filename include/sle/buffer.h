#ifndef SLE_BUFFER_H
#define SLE_BUFFER_H

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

class Buffer;
using BufferPtr = std::unique_ptr<Buffer>;

class Buffer
{
public:
    static BufferPtr create();

    virtual ~Buffer() = default;

    virtual void readFile(const std::string& path) = 0;

    virtual void saveFile(const std::string& path) = 0;

    virtual void show(Screen* scr) const = 0;

    virtual int getSize() const = 0;

    virtual const Text& getData() const = 0;

    virtual void clear() = 0;

    virtual void addLines(const Text& strs) = 0;

    virtual void addChar(const char c) = 0;

    virtual void eraseChar(const int num) = 0;

    // Move cursor functionality to its own class
    virtual void moveCursor(const int col, const int lines) = 0;

    virtual void backWord(const unsigned num) = 0;

    virtual void setX(const int value) = 0;

    virtual Coord getCursor() const = 0;

protected:
    Buffer() = default;

private:
    Buffer(const Buffer&) = delete;

    Buffer& operator=(const Buffer&) = delete;
};

} // namespace sle

#endif // SLE_BUFFER_H
