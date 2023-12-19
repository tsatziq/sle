#ifndef SLE_EDITLOOP_H
#define SLE_EDITLOOP_H

#include "sle/context.h"
#include <memory>
#include <vector>
#include <string>

namespace sle
{

class EditLoop
{
public:
    EditLoop(
        const Context& context);

    void init();

private:
    Context c_;
};

} // namespace sle

#endif // SLE_EDITLOOP_H
