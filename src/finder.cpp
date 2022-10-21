#include "sle/finder.h"
#include "sle/context.h"
#include "sle/types.h"
#include <algorithm>
#include <cctype>
#include <memory>

namespace sle
{

class FinderImpl : public Finder
{
public:
    static std::unique_ptr<FinderImpl> create(const Context* context)
    {
        return std::unique_ptr<FinderImpl>(new FinderImpl(context));
    }

    ~FinderImpl();

    Coord findWord() override;

private:
    FinderImpl(const Context* context);

    bool isWordBreak(const char c)
    {
        return !std::isalpha(c) && c != '\n' && c != '!';
    }

    const Context* c_ = nullptr;
    const Text* txt_ = nullptr;
    Cursor* cursor_ = nullptr;
};

FinderImpl::FinderImpl(const Context* context)
    : c_(context)
    , txt_(context->buffer->data())
    , cursor_(context->cursor.get())
{}

FinderPtr Finder::create(const Context* context)
{
    return FinderImpl::create(context);
}

FinderImpl::~FinderImpl()
{}

Coord FinderImpl::findWord()
{
    Coord pos = cursor_->coord();
    std::string line = txt_->at(pos.y());

    auto it = std::find_if(line.begin() + pos.x(), line.end(),
        [&](const auto& c) { return isWordBreak(c); });

     if (it == line.end())
    {
        // Last line in text, return.
        if (pos.y() == static_cast<int>(txt_->size() - 1)
            || pos.y() == c_->pager->lastLineNum())
                pos.setX(line.size() - 2);
        else
        {
        pos.setX(0);
        pos.setY(pos.y() + 1);
        }
    }
    else
    {
    // Adjust cursor position.
    pos.setX(it - line.begin());
    pos.setX(pos.x() + 1);

    if (pos.x() > static_cast<int>(line.size() - 2))
        pos.setX(line.size() - 2);
    }

    return pos;
}

}

