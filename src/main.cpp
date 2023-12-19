#include "sle/context.h"
#include "sle/buffer.h"
#include "sle/editloop.h"
#include "sle/filehandler.h"
#include "sle/mainscreen.h"
#include <memory>

using namespace sle;

int main()
{
    Context context;

    std::shared_ptr<Buffer> buf = std::make_shared<Buffer>(context);
    std::shared_ptr<EditLoop> loop = std::make_shared<EditLoop>(context);
    std::shared_ptr<FileHandler> file = std::make_shared<FileHandler>(context);
    std::shared_ptr<MainScreen> scr = std::make_shared<MainScreen>(context);

    context.buf = buf;;
    context.loop = loop;
    context.file = file;
    context.scr = scr;

    EXIT_SUCCESS;
}
