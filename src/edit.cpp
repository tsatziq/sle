#include "sle/edit.h"
#include "sle/eventaction.h"

#include "sle/screenmanager.h"
#include "sle/mainscreen.h"
#include <iostream>

namespace sle {

class Edit::Impl {
public:
    Impl(const DispatcherPtr& dispatcher);

private:
    void getInput();

    const DispatcherPtr& dispatcher_;
    EventActionContainer actions_;
};

Edit::Edit(const DispatcherPtr& dispatcher)
    : impl_(new Impl(dispatcher))
{}

Edit::~Edit()
{}

Edit::Impl::Impl(const DispatcherPtr& dispatcher)
    : dispatcher_(dispatcher), actions_(dispatcher)
{

    actions_.on<ScreensReady>([&](const ScreensReady&)
    {
//        getInput();
    });

}

void Edit::Impl::getInput()
{
    int c;
    while (true) {
        c = getch();
        std::cout << c << std::endl;
    }
}

}
