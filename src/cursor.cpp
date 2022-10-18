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

    void move(const Coord textPos, const Coord scrPos) override;

    void move(const QuickMove pos) override;

    void redraw() override;

    Coord coord() const override;

    void upDown(const int count) override;

    void leftRight(const int count) override;

    void setY(const int val) override;

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
    , scr_(context->screens->screen(ScreenId::main))
    , pager_(context->pager.get())
    , txt_(context->buffer->data())
{}

CursorPtr Cursor::create(const Context* context)
{
    return CursorImpl::create(context);
}

CursorImpl::~CursorImpl()
{}

void CursorImpl::move(const Coord textPos, const Coord scrPos)
{
    if (!textPos.isUnset())
        txtPos_ <<= textPos;
    if (!scrPos.isUnset())
        scrPos_ <<= scrPos;
    redraw();
}

void CursorImpl::move(const QuickMove pos)
{
    switch (pos)
    {
    case QuickMove::B:
    {
        move(Coord(0, 0, true, false), Coord(0, 0, true, false));
        break;
    }
    case QuickMove::C:
    {
        int center = (txt_->at(txtPos_.y()).size() - 2) / 2;
        move(Coord(center, 0, true, false), Coord(center, 0, true, false));
        break;
    }
    case QuickMove::E:
    {
        int length = txt_->at(txtPos_.y()).size() - 2;
        move(Coord(length, 0, true, false), Coord(length, 0, true, false));
        break;
    }
    case QuickMove::H:
    {
        Coord txt(0, pager_->firstLineNum());
        move(txt, Coord(0, 0));
        break;
    }
    case QuickMove::L:
    {
        Coord txt(0, pager_->lastLineNum() - 1);
        move(txt, Coord(0, pager_->numLinesOnScr() - 1));
        break;
    }
    case QuickMove::M:
    {
        int height = pager_->numLinesOnScr() - 1;
        int middle = pager_->firstLineNum() + (height / 2);
        Coord txt(0, middle);
        move(txt, Coord(0, height / 2));
        break;
    }
    default:
        break;
    }
}

void CursorImpl::redraw()
{
    wmove(scr_->curse(), scrPos_.y(), scrPos_.x());
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
    if (nScrLine >= scr_->height() || nScrLine < 0)
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
    if (nScrCol >= scr_->width() || nScrCol < 0)
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
    return yCoord - (pager_->pageNum() * scr_->height());
}

void CursorImpl::setY(const int val)
{
    txtPos_.setY(val);
}

}

