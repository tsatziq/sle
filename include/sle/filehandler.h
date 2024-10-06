#ifndef SLE_FILEHANDLER_H
#define SLE_FILEHANDLER_H

#include "sle/buffer.h"
#include "sle/context.h"
#include <memory>
#include <vector>
#include <string>

namespace sle
{

class FileHandler
{
public:
    FileHandler(
        const ContextPtr& context);

    void init();

    void readFile(
        const std::string& path,
        Buffer* buf);

    // vai string view?
    /// Returns header/source file for the current file.
    ///
    /// \return Empty if file not found, hdr if current file is source, etc.
    const std::string getFilePair();

private:
    ContextPtr c_ = nullptr;
    std::string path_;
};

} // namespace sle

#endif // SLE_FILEHANDLER_H
