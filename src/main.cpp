#include "sle/context.h"
#include "sle/screenmanager.h"
#include "sle/buffer.h"
#include "sle/chorder.h"
#include "sle/cursor.h"
#include "sle/finder.h"
#include "sle/pager.h"
#include "sle/sidebar.h"
#include "sle/modeloop.h"
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
    context.pager = Pager::create(&context);
    context.cursor = Cursor::create(&context);
    context.finder = Finder::create(&context);
    context.chorder = Chorder::create(&context);

    context.buffer->readFile(file);
    context.pager->show();
    context.sideBar->refresh();

    ModeLoopPtr modeLoop{ModeLoop::create(&context)};
    modeLoop->start();

    return EXIT_SUCCESS;
}
