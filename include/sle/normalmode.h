#ifndef SLE_NORMALMODE_H
#define SLE_NORMALMODE_H

#include "sle/dispatcher.h"
#include "sle/screen.h"

namespace sle {

class NormalMode
{
public:
    NormalMode(const DispatcherPtr& dispatcher, const ScreenPtr& scr);

    ~NormalMode();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace sle

#endif // SLE_NORMALMODE_H
