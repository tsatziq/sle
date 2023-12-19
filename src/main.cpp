#include "sle/context.h"
#include "sle/buffer.h"
#include "sle/editloop.h"
#include "sle/filehandler.h"
#include "sle/mainscreen.h"
#include <memory>

using namespace sle;

int main()
{
    ContextPtr context = std::make_shared<Context>();

    Buffer buf(context);
    EditLoop loop(context);
    FileHandler file(context);
    MainScreen scr(context);

    context->buf = &buf;
    context->loop = &loop;
    context->file = &file;
    context->scr = &scr;

    scr.init();
    loop.init();

    EXIT_SUCCESS;
}
