#include "sle/sidebar.h"
#include "sle/context.h"
#include "sle/screen.h"
#include <map>
#include <ncurses.h>

namespace sle {

class SideBarImpl : public SideBar
{
public:
    static std::unique_ptr<SideBarImpl> create(
        Screen* side, Screen* main, Buffer* buf)
    {
        return std::unique_ptr<SideBarImpl>(
            new SideBarImpl(side, main, buf));
    }

    ~SideBarImpl();

    void refresh() override;

private:
    SideBarImpl(Screen* side, Screen* main, Buffer* buf);

    Screen* sideScr_;
    Screen* mainScr_;
    Buffer* buf_;
};

SideBarImpl::SideBarImpl(Screen* side, Screen* main, Buffer* buf)
    : sideScr_(side), mainScr_(main), buf_(buf)
{}

SideBarPtr SideBar::create(Screen* side, Screen* main, Buffer* buf)
{
    return SideBarImpl::create(side, main, buf);
}

SideBarImpl::~SideBarImpl()
{}

void SideBarImpl::refresh()
{
    int size = buf_->getSize();
    if (size == 0)
        size = 1;

    std::vector<std::string> lineNums;
    std::string line;

    for (int i = 1; i <= size; i++) {
        line = "   " + std::to_string(i) + "\n";
        lineNums.push_back(line);
    }

    int h = sideScr_->getHeight();
    h -= lineNums.size();

    for (int i = 1; i <= h; i++) {
        line = "~\n";
        lineNums.push_back(line);
    }

    sideScr_->paint(lineNums);

    Coord cursor = buf_->getCursor();
    wmove(mainScr_->getCurse(), cursor.y, cursor.x);
}

}
