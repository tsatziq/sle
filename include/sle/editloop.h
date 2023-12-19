#ifndef SLE_EDITLOOP_H
#define SLE_EDITLOOP_H

#include "sle/context.h"
#include "sle/buffer.h"
#include <memory>
#include <vector>
#include <string>

namespace sle
{

class EditLoop
{
public:
    EditLoop(
        const ContextPtr& context);

    void init();

private:
    ContextPtr c_ = nullptr;
};

} // namespace sle

#endif // SLE_EDITLOOP_H
