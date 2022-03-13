#ifndef SLE_FILEHANDLER_H
#define SLE_FILEHANDLER_H

#include "sle/dispatcher.h"
#include "sle/linetype.h"
#include <memory>
#include <vector>
#include <string>

namespace sle {

class FileHandler;
using FileHandlerPtr = std::unique_ptr<FileHandler>;

class FileHandler
{
public:
    static FileHandlerPtr create();

    virtual ~FileHandler() = default;

    virtual StrPacket getContents(const std::string& path) const = 0;

    virtual void saveFile(const std::string& path) const = 0;

protected:
    FileHandler() = default;
};

} // namespace sle

#endif // SLE_FILEHANDLER_H
