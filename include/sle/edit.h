#ifndef SLE_EDIT_H
#define SLE_EDIT_H

#include "sle/dispatcher.h"

namespace sle {

class Edit;
using EditPtr = std::unique_ptr<Edit>;

class Edit {
public:
    Edit(const DispatcherPtr& dispatcher);

    ~Edit();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace sle

#endif // SLE_EDIT_H
