#include "sle/finder.h"
#include "sle/context.h"
#include "sle/types.h"
#include <algorithm>
#include <cctype>
#include <iterator>
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

    Coord nextWord() override;

    Coord prevWord() override;

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

Coord FinderImpl::nextWord()
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
    pos.setX(std::distance(line.begin(), it));
    pos.setX(pos.x() + 1);

    if (pos.x() > static_cast<int>(line.size() - 2))
        pos.setX(line.size() - 2);
    }

    return pos;
}

Coord FinderImpl::prevWord()
{
    Coord pos = cursor_->coord();

    std::string line = txt_->at(pos.y());
    int rX = (line.size() - 1) - pos.x(); // Index in reverse

    auto end = std::reverse_iterator(line.rend());
    auto begin = std::reverse_iterator(line.rbegin());
    auto posNow = std::reverse_iterator(line.rbegin() + rX);

    // If already at start of word, find prev word first.
    if (*(posNow + 1) == ' ')
    {
        ++posNow;
        while (*posNow == ' ')
            ++posNow;
    }

    rX = std::distance(line.rbegin(), posNow);

    auto rit = std::find_if(begin + rX, end,
        [&](const auto& c) { return isWordBreak(c); });

    if (rit == end)
    {
        // First line in text, return.
        if (pos.y() == 0 || pos.y() == c_->pager->firstLineNum())
            pos.setX(0);
        // Started at beginning of line, skip to end of previous line.
        else if (pos.x() == 0)
        {
            pos.setY(pos.y() - 1);
            pos.setX(txt_->at(pos.y()).size() - 2);
        }
        // Started in first word, go to beginning of current line.
        else
            pos.setX(0);
    }
    else
    {
        // Adjust cursor position.
        rX = std::distance(line.rbegin(), rit);
        pos.setX(line.size() - rX);

        if (pos.x() < 0)
            pos.setX(0);
    }

    return pos;
}

}

