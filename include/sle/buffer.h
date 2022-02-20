#ifndef SLE_BUFFER_H
#define SLE_BUFFER_H

#include "sle/dispatcher.h"
#include "sle/screen.h"
#include <map>
#include <memory>

namespace sle {

enum class BufferId : unsigned;

// MUTTA PERKELE IHAN SAMA ALOTA PERUSVERSIOSTA ELI:
// Filehandlerilla on YKS file ja YKS bufferi.
// sillo destructor aina clearaa sisallon jne, kutsu .reset(new Buffer)
// myohemmin tee vaik FileManager-Filehandler-Buffer

// FileHandler should take care of not sending too much data at once,
// it can construct with part and append afterwards if needed.

class Buffer;
using BufferPtr = std::unique_ptr<Buffer>;

class Buffer {
public:
    Buffer(const DispatcherPtr& dispatcher, const StrPacket& text);

    ~Buffer();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace sle

#endif // SLE_BUFFER_H
