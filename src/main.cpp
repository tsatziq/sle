#include "sle/test.h"
#include "sle/dispatcher.h"
#include "sle/screenmanager.h"
#include "sle/filehandler.h"
#include "sle/edit.h"
#include <cstdlib>

using namespace sle;

int main()
{
    DispatcherPtr dispatcher = Dispatcher::createDefault();
    //Tester test = Tester(dispatcher);
    //dispatcher->sendEvent(TestEvent());

    FileHandler fileHandler = FileHandler(dispatcher, "testtxt");
    EditPtr editLoop = std::make_unique<Edit>(dispatcher);
	ScreenManagerPtr scrManager = ScreenManager::create(dispatcher);
    
    dispatcher->dispatch();
    // varmista etta jatkossakin valittaa eventit

    EXIT_SUCCESS;
}
