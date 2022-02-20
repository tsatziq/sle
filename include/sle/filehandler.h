#ifndef SLE_FILEHANDLER_H
#define SLE_FILEHANDLER_H

#include "sle/dispatcher.h"
#include <memory>
#include <vector>
#include <string>

namespace sle {
// TAMA EI EHKA TOIMIKAA FSMna, laita mielummi etta
// objektin luominen jo avaa ja tekee bufferin.

/// Copy file contents to buffer.
// TODO: Add functionality later to open in same or new buffer
struct OpenFile {
    std::string path;
};

/// Update the file with buffer contents and save it.
struct SaveFile {
    std::string path;
};

class FileHandler;
using FileHandlerPtr = std::shared_ptr<FileHandler>;

class FileHandler
{
public:
    FileHandler(
        const DispatcherPtr& dispatcher,
        std::string path);

    ~FileHandler();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

// Later maybe multiple filehandles if opening many files
// or to save backup and file at same time, etc.

} // namespace sle

#endif // SLE_FILEHANDLER_H
