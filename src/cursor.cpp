#include "sle/cursor.h"
#include "sle/screenmanager.h"
#include "sle/types.h"
#inclue <memory>

namespace sle {

class CursorImpl : public Cursor
{
public:
    static std::unique_ptr<CursorImpl> create(
        const ScreenManager* scrs, const Text& txt)
    {
        return std::unique_ptr<CursorImpl>(new CursorImpl(scrs, txt));
    }

    ~CursorImpl();

    void move(const Coord to, bool setX, bool setY) override;

    void up(const unsigned count) override;

    void down(const unsigned count) override;

    void back(const unsigned count) override;

    void forward(const unsigned count) override;

    void backWord(const unsigned num) override;

    Coord coord() const override;

private:
    CursorImpl(const ScreenManager* scrs, const Text& txt);

    const ScreenManager* scrs_;
    const Text::iterator txt_;
    Coord point_{0, 0};
};

CursorImpl::CursorImpl(const ScreenManager* scrs, const Text& txt)
    : scrs_(scrs), txt_(txt.begin())
{}

CursorPtr Cursor::create(const ScreenManager* scrs, const Text& txt)
{
    return CursorImpl::create(scrs, txt);
}

CursorImpl::~CursorImpl)
{}

void CursorImpl::move(const Coord to, bool setX, bool setY)
{
    if (setX)
        point_.x = to.x;
    if (setY)
        point_.y = to.y;
}

void CursorImpl::up(const unsigned count)
{
    if (point_.y != 0)
        point_.y -= count;
}

void CursorImpl::down(const unsigned count)
{
    point_.y += count;
}

void CursorImpl::back(const unsigned count)
{
    point_.x -= count;
}

void CursorImpl::forward(const unsigned count)
{
    point_.x += count;
}

Coord CursorImpl::coord const
{
    return point_;
}

}

