#include "sle/cursor.h"
#include "sle/context.h"
#include "sle/screen.h"
#include "sle/types.h"
#include <memory>

namespace sle {

class CursorImpl : public Cursor
{
public:
    static std::unique_ptr<CursorImpl> create(
        const Context* context)
    {
        return std::unique_ptr<CursorImpl>(new CursorImpl(context));
    }

    ~CursorImpl();

    void move(const Coord to) override;

    void redraw() override;

    Coord coord() const override;

    void upDown(const int count) override;

    void leftRight(const int count) override;

private:
    CursorImpl(const Context* context);

    const Context* c_ = nullptr;
    Screen* scr_ = nullptr;
    const Text* txt_ = nullptr;
    Coord point_{0, 0, true, true};
};

CursorImpl::CursorImpl(const Context* context)
    : c_(context)
    , scr_(context->screens->getScreen(ScreenId::main))
    , txt_(context->buffer->getData())
{}

CursorPtr Cursor::create(const Context* context)
{
    return CursorImpl::create(context);
}

CursorImpl::~CursorImpl()
{}

void CursorImpl::move(const Coord to)
{
    point_ = to;
    redraw();
}

void CursorImpl::redraw()
{
    wmove(scr_->getCurse(), point_.y, point_.x);
}

Coord CursorImpl::coord() const
{
    return point_;
}

void CursorImpl::upDown(const int count)
{
    int size = txt_->size();
    int newLine = point_.y + count;
    if (newLine < 0 || newLine >= size)
        return;

    point_.y = newLine;
    redraw();
}

void CursorImpl::leftRight(const int count)
{
    int size = txt_->at(point_.y).size() - 1;
    int newCol = point_.x + count;
    if (newCol < 0 || newCol >= size)
        return;

    point_.x = newCol;
    redraw();
}

}

