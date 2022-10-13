#include "sle/cursor.h"
#include "sle/coord.h"
#include "sle/context.h"
#include "sle/pager.h"
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

    int txtToScrCoord(int yCoord);

    const Context* c_ = nullptr;
    Screen* scr_ = nullptr;
    Pager* pager_ = nullptr;
    const Text* txt_ = nullptr;
    Coord txtPos_{0, 0, true, true};
    Coord scrPos_{0, 0, true, true};
};

CursorImpl::CursorImpl(const Context* context)
    : c_(context)
    , scr_(context->screens->getScreen(ScreenId::main))
    , pager_(context->pager.get())
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
    txtPos_ <<= to;
    scrPos_.setY(0);
    redraw();
}

void CursorImpl::redraw()
{
    wmove(scr_->getCurse(), scrPos_.y(), scrPos_.x());
}

Coord CursorImpl::coord() const
{
    return txtPos_;
}

void CursorImpl::upDown(const int count)
{
    int size = txt_->size();

    // Check if the move is legal in scr.
    int nScrLine = scrPos_.y() + count;
    if (nScrLine >= scr_->getHeight() || nScrLine < 0)
        return;

    // Then check if it is legal in the text.
    int nTxtLine = txtPos_.y() + count;
    if (nTxtLine >= size || nTxtLine < 0)
        return;

    // All good to move.
    txtPos_.setY(nTxtLine);
    scrPos_.setY(nScrLine);

    // Adjust x.
    int nLineLength = txt_->at(nTxtLine).size() - 1;
    if (txtPos_.x() >= nLineLength)
    {
        txtPos_.setX(nLineLength - 1);
        scrPos_.setX(nLineLength - 1);
    }

    redraw();
}

void CursorImpl::leftRight(const int count)
{
    // Check if the move is legal in scr.
    int nScrCol = scrPos_.x() + count;
    if (nScrCol >= scr_->getWidth() || nScrCol < 0)
        return;

    // Then check if it is legal in the text.
    int nTxtCol = txtPos_.x() + count;
    int length = txt_->at(txtPos_.y()).size() - 1;
    if (nTxtCol >= length || nTxtCol < 0)
        return;

    txtPos_.setX(nTxtCol);
    scrPos_.setX(nScrCol);
    redraw();
}

int CursorImpl::txtToScrCoord(int yCoord)
{
    return yCoord - (pager_->getPageNum() * scr_->getHeight());
}

}

