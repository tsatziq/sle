#include "sle/screen.h"
#include "sle/types.h"
#include <memory>
#include <ncurses.h>

namespace sle {

class ScreenImpl : public Screen
{
public:
    static std::unique_ptr<ScreenImpl> create(int height, int width, const Coord& start)
    {
        return std::unique_ptr<ScreenImpl>(
            new ScreenImpl(height, width, start));
    }

    ~ScreenImpl();

    void paint(const Text& txt) override;

    void paint(const char c) override;

    int getChar() const override;

    std::string getLine() const override;

    CursePtr getCurse() const override;

    int getHeight() const override;

    int getWidth() const override;

private:
    explicit ScreenImpl(int height, int width, const Coord& start)
        : height_(height), width_(width), start_(start), curse_(nullptr)
    {
        curse_ = newwin(height, width, start.y, start.x);
    }

    ScreenImpl(const ScreenImpl&) = delete;
    ScreenImpl& operator=(ScreenImpl&) = delete;

    ScreenImpl(const ScreenImpl&&) = delete;
    ScreenImpl& operator=(ScreenImpl&&) = delete;

    Coord start_;
    int height_, width_;
    CursePtr curse_;
};

ScreenPtr Screen::create(int height, int width, const Coord& start) {
    return ScreenImpl::create(height, width, start);
}

ScreenImpl::~ScreenImpl()
{
    delwin(curse_);
}

void ScreenImpl::paint(const Text& txt)
{
    werase(curse_);
    for (std::string str : txt)
        wprintw(curse_, str.c_str());
    wrefresh(curse_);
}

void ScreenImpl::paint(const char c)
{
    waddch(curse_, c);
    wrefresh(curse_);
}

int ScreenImpl::getChar() const
{
    return wgetch(curse_);
}

std::string ScreenImpl::getLine() const {
    return "";    
}

CursePtr ScreenImpl::getCurse() const {
    return curse_;
}

int ScreenImpl::getHeight() const
{
    return height_;
}

int ScreenImpl::getWidth() const
{
    return width_;
}

}
