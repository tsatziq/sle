#include "sle/test.h"
#include <iostream>

namespace sle
{

Tester::Tester(
    const DispatcherPtr& dispatcher) :
x_(0),
dispatcher_(dispatcher),
actions_(dispatcher)
{
    actions_.on<TestEvent>(
        [&](const TestEvent&)
    {
        std::cout << "Test event received\n";
    });
}

void Tester::sayHi()
{
    std::cout << "HELLO\n";
}

}
