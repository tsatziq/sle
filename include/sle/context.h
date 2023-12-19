#ifndef SLE_CONTEXT_H
#define SLE_CONTEXT_H

#include <memory>

namespace sle
{

class Buffer;
class EditLoop;
class FileHandler;
class MainScreen;

using BufferPtr = std::shared_ptr<Buffer>;
using EditLoopPtr = std::shared_ptr<EditLoop>;
using FileHandlerPtr = std::shared_ptr<FileHandler>;
using MainScreenPtr = std::shared_ptr<MainScreen>;

struct Context
{
    BufferPtr buf;
    EditLoopPtr loop;
    FileHandlerPtr file;
    MainScreenPtr scr;
};

} // namespace sle

#endif // SLE_CONTEXT_H
