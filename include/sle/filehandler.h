#ifndef SLE_FILEHANDLER_H
#define SLE_FILEHANDLER_H

//#include "sle/dispatcher.h"
#include "sle/linetype.h"
#include <memory>
#include <vector>
#include <string>

namespace sle {

class FileHandler;
using FileHandlerPtr = std::unique_ptr<FileHandler>;

class FileHandler {
public:
    static FileHandlerPtr create(std::string path);
    
    virtual ~FileHandler() = default;
    
    virtual StrPacket getContents() = 0;
    
    virtual void saveFile() = 0;
    
    virtual void changePath(std::string path) = 0;

protected:
    FileHandler() = default;
};

// Later maybe multiple filehandles if opening many files
// or to save backup and file at same time, etc.

} // namespace sle

#endif // SLE_FILEHANDLER_H
