#ifndef SLE_BUFFER_H
#define SLE_BUFFER_H

#include <map>
#include <memory>

namespace sle {

class Buffer;
using BufferPtr = std::unique_ptr<Buffer>;

class Buffer {
public:
    Buffer();

    ~Buffer();

private:
    //void addToScreen(const LineNum from);
    /* 
    std::map<LineNum, std::string> lines_;
    LineNum topVisibleLine_;
    LineNum screenHeight_;
    bool modified_;
    */
};

} // namespace sle

#endif // SLE_BUFFER_H
