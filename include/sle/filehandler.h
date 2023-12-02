#ifndef SLE_FILEHANDLER_H
#define SLE_FILEHANDLER_H

#include <memory>
#include <vector>
#include <string>

namespace sle {

class FileHandler;
using FileHandlerPtr = std::shared_ptr<FileHandler>;

class FileHandler
{
public:
    FileHandler()
    {}

    ~FileHandler();

private:
};

} // namespace sle

#endif // SLE_FILEHANDLER_H
