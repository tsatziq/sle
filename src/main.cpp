#include "sle/dispatcher.h"
#include "sle/screenmanager.h"
#include <cstdlib>

#include "sle/buffer.h"
#include "sle/screen.h"
#include <memory>

using namespace sle;

int main()
{
    DispatcherPtr dispatcher = Dispatcher::createDefault();

	ScreenManagerPtr scrManager = ScreenManager::create(dispatcher);
    ScreenPtr mainScr = scrManager->getScreenPtr(ScreenId(1));
    BufferPtr buffer = std::make_shared<Buffer>(dispatcher, "testtxt", mainScr);
    
    dispatcher->dispatch();
    // varmista etta jatkossakin valittaa eventit

    EXIT_SUCCESS;
}
