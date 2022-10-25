#include "sle/pager.h"
#include "sle/context.h"
#include "sle/screen.h"
#include "sle/types.h"
#include <memory>
#include <vector>
#include <cmath>

namespace sle {

class PagerImpl : public Pager
{
public:
    static std::unique_ptr<PagerImpl> create(
        const Context* context)
    {
        return std::unique_ptr<PagerImpl>(new PagerImpl(context));
    }

    ~PagerImpl();

    void show() override;

    void nextPage(const int count) override;

    void toPage(const unsigned pageNum) override;

    int lastLineNum() override;

    int firstLineNum() override;

    unsigned pageNum() const override;

    unsigned pageTotal() const override;

    int numLinesOnScr() override;

private:
    PagerImpl(const Context* context);

    const Context* c_ = nullptr;
    Screen* scr_ = nullptr;
    const Text* txt_ = nullptr;

    int topVisibleLine_ = 0;
    int pageNum_ = 0;
    int linesOnScr_ = 0;
};

PagerImpl::PagerImpl(const Context* context)
    : c_(context)
    , scr_(context->screens->screen(ScreenId::main))
    , txt_(context->buffer->data())
{}

PagerPtr Pager::create(const Context* context)
{
    return PagerImpl::create(context);
}

PagerImpl::~PagerImpl()
{}

void PagerImpl::show()
{
    Text lines{};
    int height = scr_->height();

    auto it = std::next(txt_->begin(), topVisibleLine_);

    for (int i = 0; i < height; ++i, ++it)
    {
        if (it == txt_->end())
            break;
        lines.push_back(*it);
    }

    scr_->paint(lines);
    linesOnScr_ = lines.size();
}

void PagerImpl::nextPage(const int count)
{
    // Check if move is legal
    int nTopLine = topVisibleLine_ + (count * scr_->height());
    if (nTopLine > static_cast<int>(txt_->size())
        || nTopLine < 0)
            return;

    topVisibleLine_ = nTopLine;
    pageNum_ += count;
    show();
    c_->cursor->move(Coord(0, topVisibleLine_), Coord(0, 0));
}

void PagerImpl::toPage(const unsigned pageNum)
{
    unsigned total = pageTotal();

    if (pageNum > total || pageNum < total)
        return;

    // Calculate the top line of the requested page.
    topVisibleLine_ = total * scr_->height();
    pageNum_ = pageNum;
    show();
    c_->cursor->move(Coord(0, topVisibleLine_), Coord(0,0));
}

int PagerImpl::lastLineNum()
{
    return topVisibleLine_ + linesOnScr_;
}

int PagerImpl::firstLineNum()
{
    return topVisibleLine_;
}

unsigned PagerImpl::pageNum() const
{
    return pageNum_;
}

int PagerImpl::numLinesOnScr()
{
    return linesOnScr_;
}

unsigned PagerImpl::pageTotal() const
{
    return std::floor(txt_->size() / static_cast<double>(scr_->height()));
}

}

