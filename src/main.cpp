#include "sle/context.h"
#include "sle/screenmanager.h"
#include "sle/buffer.h"
#include "sle/sidebar.h"
//#include "sle/modeloop.h"
#include <cstdlib>

using namespace sle;

int main(int argc, char** argv)
{
    std::string file = "";
    if (argc > 1)
        file = argv[1];
    
    Context context{}; 

    context.screens = ScreenManager::create(&context);
    context.buffer = Buffer::create(&context);
    context.sideBar = SideBar::create(&context);

    //ModeLoopPtr modeLoop{ModeLoop::create(context, file)};
    //modeLoop->start();

    return EXIT_SUCCESS;
}
