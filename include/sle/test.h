#include "sle/dispatcher.h"
#include "sle/eventaction.h"

namespace sle
{

struct TestEvent
{};

class Tester
{
public:
    Tester(
        const DispatcherPtr& dispatcher);

    void sayHi();

private:
    int x_;
    const DispatcherPtr dispatcher_;
    EventActionContainer actions_;
};

}
