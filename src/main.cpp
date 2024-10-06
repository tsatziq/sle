#include "sle/context.h"
#include "sle/buffer.h"
#include "sle/editloop.h"
#include "sle/filehandler.h"
#include "sle/mainscreen.h"
#include <memory>

using namespace sle;

int main(int argc, char* argv[])
{
    ContextPtr context = std::make_shared<Context>();

    Buffer buf(context);
    Buffer prev(context);
    EditLoop loop(context);
    FileHandler file(context);
    MainScreen scr(context);

    context->buf = &buf;
    context->prev = &prev;
    context->loop = &loop;
    context->file = &file;
    context->scr = &scr;

    scr.init();

    if (argc > 1)
    {
        file.readFile(std::string(argv[1]), &buf);
        if (!buf.getText().empty())
        {
            scr.paint(buf.getText());
            if (buf.size() > scr.height())
                context->visibleRange->end()->setY(scr.height() - 1);
            else
                context->visibleRange->end()->setY(buf.size() - 1);
        }
        scr.moveCursor(Point::make(0, 0));
    }

    loop.run();

    EXIT_SUCCESS;
}
