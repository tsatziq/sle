#include "sle/dispatcher.h"
#include "sle/screen.h"
#include "sle/screenmanager.h"
#include "sle/filehandler.h"
#include "sle/buffermanager.h"
#include "sle/buffer.h"
#include "sle/normalmode.h"
#include <cstdlib>

using namespace sle;

int main()
{
    DispatcherPtr dispatcher = Dispatcher::createDefault();

    BufferManagerPtr bufManager = BufferManager::create(dispatcher);
	ScreenManagerPtr scrManager = ScreenManager::create(dispatcher);

    BufferId buf = bufManager->addBuffer();
    //BufferId id = BufferId(1);
    //BufferPtr buf = std::make_shared<Buffer>(dispatcher, id);
    ScreenPtr main = scrManager->getScreen(ScreenId::main);

    NormalMode nmode = NormalMode(dispatcher, main); 

    //dispatcher->sendEvent(ReadFile({buf, "testtxt"}));
    //dispatcher->sendEvent(ShowBuffer({buf, main}));
    //dispatcher->sendEvent(StartNormalMode());
    
    dispatcher->dispatch();

    return EXIT_SUCCESS;
}
