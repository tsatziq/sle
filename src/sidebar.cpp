#include "sle/sidebar.h"
#include "sle/context.h"
#include "sle/screen.h"
#include "sle/types.h"
#include <map>
#include <ncurses.h>

namespace sle {

class SideBarImpl : public SideBar
{
public:
    static std::unique_ptr<SideBarImpl> create(const Context* context)
    {
        return std::unique_ptr<SideBarImpl>(new SideBarImpl(context));
    }

    ~SideBarImpl();

    void refresh() override;

private:
    SideBarImpl(const Context* context);

    const Context* c_ = nullptr;
    Screen* scr_ = nullptr;
};

SideBarImpl::SideBarImpl(const Context* context)
    : c_(context)
    , scr_(c_->screens->screen(ScreenId::side))
{}

SideBarPtr SideBar::create(const Context* context)
{
    return SideBarImpl::create(context);
}

SideBarImpl::~SideBarImpl()
{}

void SideBarImpl::refresh()
{
    int size = c_->buffer->size();
    if (size == 0)
        size = 1;

    std::vector<std::string> lineNums;
    std::string line;

    for (int i = 1; i <= size; i++) {
        line = "   " + std::to_string(i) + "\n";
        lineNums.push_back(line);
    }

    int h = scr_->height();
    h -= lineNums.size();

    for (int i = 1; i <= h; i++) {
        line = "~\n";
        lineNums.push_back(line);
    }

    scr_->paint(lineNums);
    c_->cursor->redraw();
}

}
