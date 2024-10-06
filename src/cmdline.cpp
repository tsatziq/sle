#include "sle/cmdline.h"

namespace sle
{

CmdLine::CmdLine(
    ContextPtr& context)
    :
    Screen(),
    c_(context)
{}

CmdLine::~CmdLine()
{
    del_panel(panel_);
}

void CmdLine::init()
{
    height_ = 1; // DEBUG
    width_ = 80; // DEBUG
    //auto startY = c_->scr->height() - 2;
    auto startY = 22;
    scr_ = newwin(height_, width_, 22, 0);
    panel_ = new_panel(scr_);

    // Update here after scr is created too.
    update_panels();
    doupdate();
}

const PointPtr& CmdLine::cursor() const
{
    return cursor_;
}

void CmdLine::paintLn(
    const std::string str)
{}

bool CmdLine::paintCh(
    const char c,
    const bool replace)
{
    if ((cursor_->x() < width_) && !replace)
        mvwinsch(scr_, cursor_->y(), cursor_->x(), c);
    else
        mvwaddch(scr_, cursor_->y(), cursor_->x(), c);
    cursor_->incX();

    if (cursor_->x() < width_)
        moveCursor(cursor_);
    return true;
}

void CmdLine::moveCursor(
    const PointPtr& point)
{
    if (*point == *cursor_)
        return;
    if (point->x() < 0 || point->y() < 0)
        return;
    if (point->x() > width_ || point->y() > height_)
        return;

    cursor_ = point;
    wmove(scr_, point->y(), point->x());
}

int CmdLine::getCh()
{
    return wgetch(scr_);
}

void CmdLine::clear()
{}

void CmdLine::update()
{
    update_panels();
    doupdate();
}

}
