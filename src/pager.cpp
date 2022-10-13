#include "sle/pager.h"
#include "sle/context.h"
#include "sle/screen.h"
#include "sle/types.h"
#include <memory>
#include <vector>

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

    void movePage(const int count) override;

    int lastLineNum() override;

    int firstLineNum() override;

    int getPageNum() override;

private:
    PagerImpl(const Context* context);

    const Context* c_ = nullptr;
    Screen* scr_ = nullptr;
    const Text* txt_ = nullptr;

    int topVisibleLine = 0;
    int pageNum = 0;
};

PagerImpl::PagerImpl(const Context* context)
    : c_(context)
    , scr_(context->screens->getScreen(ScreenId::main))
    , txt_(context->buffer->getData())
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
    int height = scr_->getHeight();

    auto it = std::next(txt_->begin(), topVisibleLine);

    for (int i = 0; i < height; ++i, ++it)
    {
        if (it == txt_->end())
            break;
        lines.push_back(*it);
    }

    scr_->paint(lines);
}

void PagerImpl::movePage(const int count)
{
    // Check if move is legal
    int nTopLine = topVisibleLine + (count * scr_->getHeight());
    if (nTopLine > static_cast<int>(txt_->size())
        || nTopLine < 0)
            return;

    topVisibleLine = nTopLine;
    pageNum += count;
    show();
    c_->cursor->move(Coord(0, topVisibleLine, false, true));
}

int PagerImpl::lastLineNum()
{
    return topVisibleLine + scr_->getHeight();
}

int PagerImpl::firstLineNum()
{
    return topVisibleLine;
}

int PagerImpl::getPageNum()
{
    return pageNum;
}

}

