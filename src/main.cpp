#include "sle/context.h"
#include "sle/modeloop.h"
#include <cstdlib>

using namespace sle;

int main(int argc, char** argv)
{
    ContextPtr context = std::make_shared<Context>(); 

    std::string file = "";
    if (argc > 1)
        file = argv[1];

    ModeLoopPtr modeLoop{ModeLoop::create(context, file)};
    modeLoop->start();

    return EXIT_SUCCESS;
}
