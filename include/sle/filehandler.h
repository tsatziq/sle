#ifndef SLE_FILEHANDLER_H
#define SLE_FILEHANDLER_H

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
        const Context& context);

    void init();

private:
    Context c_;
};

} // namespace sle

#endif // SLE_FILEHANDLER_H
