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

private:
    ContextPtr c_ = nullptr;
};

} // namespace sle

#endif // SLE_FILEHANDLER_H
