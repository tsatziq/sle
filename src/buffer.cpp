#include "sle/buffer.h"
#include <algorithm>
#include <iterator>
#include <memory>
#include <string_view>
#include <regex>

namespace sle
{

Buffer::Buffer(
    const ContextPtr& context)
    :
    c_(context)
{}

void Buffer::init()
{
}

void Buffer::addCh(
    const char ch,
    const bool replace)
{
    if (point_->y() < 0)
        return;
    if (point_->y() > txt_.size() - 1)
        return;

    if (!replace)
        txt_.at(point_->y()).insert(point_->x(), 1, ch);
    else
        txt_.at(point_->y())[point_->x()] = ch;

    if (ch != '\n')
        point_->incX();
    else
    {
        point_->incX();
        std::string cutText = getLine(point_);

        if (!cutText.empty())
            erase(Range::make(
                point_,
                Point::make(lineLen(point_), point_->y())));

        txt_.insert(txt_.begin() + point_->y() + 1, cutText);
        point_->set(0, point_->y() + 1);
    }
}

void Buffer::addText(
    const std::string& txt)
{
}

void Buffer::addText(
    const std::vector<std::string>& txt)
{
    if ((txt_.size() == 1) && txt_.front().empty())
        txt_.clear();
    txt_.insert(txt_.end(), txt.begin(), txt.end());
}

const std::vector<std::string>& Buffer::getText()
{
    return txt_;
}

// saisko tan joskus muutettua ettei kayttais tota fittosize? tjsp etta
// vois antaa const RangePtr& parametrin. kai sen voi nyt antaa olla.
// kylha taa voi sit ottaa fitToSize tyyppisesti oikeat rivit vektoriin, mut
// ei sen tarttis tota rangea muokata, se on rumaa.
std::vector<std::string> Buffer::getRange(
    const RangePtr& range)
{
    if (txt_.empty())
        return {};

    // Make copy so that the original is not modified.
    Range r = *range;
    auto s = r.start();
    auto e = r.end();

    if (*r.start() > *r.end())
        r.sortRange();
        
    std::vector<std::string> newVec;
    s = r.start();
    e = r.end();
    
    std::copy(
        txt_.begin() + (r.start()->y()),
        txt_.begin() + (r.end()->y() + 1),
        std::back_inserter(newVec));

    if (newVec.empty())
        return {};

    if (r.start()->isXSet())
    {
        std::string& firstLn = newVec.front();

        if (firstLn.length() >= r.start()->x()) 
            firstLn = firstLn.substr(r.start()->x());
    }

    if (r.end()->isXSet())
    {
        std::string& lastLn = newVec.back();

        if (lastLn.length() >= r.end()->x()) 
            lastLn = lastLn.substr(r.end()->x());
    }

   return newVec;
}

const PointPtr& Buffer::move(
    const Direction& dir,
    const unsigned count)
{
    switch (dir)
    {
    case Direction::LEFT:
        if (point_->x() > count)
            point_->setX(point_->x() - count);
        else
            point_->setX(0);
        break;
    case Direction::RIGHT:
    {
        auto length = lineLen(point_);

        if ((point_->x() + count) > length - 1)
        {
            // If last char is newline.
            if (txt_.at(point_->y()).back() == '\n')
                point_->setX(length - 2);
            else
                point_->setX(length - 1);
        }
        else
        {
            if (txt_.at(point_->y()).at(point_->x() + count) == '\n')
                point_->setX(point_->x() + count - 1);
            else
                point_->setX(point_->x() + count);

        }
        break;
    }
    case Direction::UP:
    {
        if (point_->y() > count)
            point_->setY(point_->y() - count);
        else
            point_->setY(0);

        auto nextLnLen = lineLen(point_);

        if (nextLnLen - 1 <= point_->x())
        {
            if (nextLnLen == 1)
                point_->setX(0);
            else if (txt_.at(point_->y()).back() == '\n')
                point_->setX(nextLnLen - 2);
            else
                point_->setX(nextLnLen - 1);
        }
        break;
    }
    case Direction::DOWN:
    {
        if (point_->y() + count > size() - 1)
            point_->setY(size() - 1);
        else
            point_->setY(point_->y() + count);

        auto nextLnLen = lineLen(point_);

        if (nextLnLen - 1 <= point_->x())
        {
            if (nextLnLen == 1)
                point_->setX(0);
            else if (txt_.at(point_->y()).back() == '\n')
                point_->setX(nextLnLen - 2);
            else
                point_->setX(nextLnLen - 1);
        }
        break;
    }
    default:
        break;
    }

    return point_;
} 

PointPtr Buffer::moveWord(
    const Direction& dir,
    const unsigned count)
{
    auto p = Point::make(point_);
    auto len = lineLen() - 2;
    auto origY = p->y();

    switch (dir)
    {
    case Direction::RIGHT:
    {
        // 1. If at file end, return.
        if (p->y() == size() - 1 && p->x() == len)
            return nullptr;

        // 2. If at line end, start from next line.
        if (p->x() == len)
        {
            p->set(0, p->y() + 1);
            len = lineLen() - 2;
        }
        
        auto pp = Point::make(p);
        pp->setX(0);
        auto str = getLine(pp);

        // 3. Skip non-whitespace (if line was not changed).
        if (p->y() == origY)
            while (p->x() < len && !std::isspace(charAt(p)))
                p->incX();

        // 4. Skip whitespace.
        while (p->x() < len && std::isspace(charAt(p)))
            p->incX();

        return p;
        break;
    }
    case Direction::LEFT:
    {
        // 1. If at file start, return.
        if (p->y() == 0 && p->x() == 0)
            return nullptr;

        // 2. Skip whitespace #1.
        if (p->x() > 0 && p->x() != 1)
            p->decX();
        while (p->x() > 0 && std::isspace(charAt(p)))
            p->decX();
        
        // 3. If at line start, move to end of previous line.
        if (p->x() == 0)
        {
            p->set(txt_.at(p->y() - 1).size() - 1, p->y() - 1);
            len = lineLen(p) - 2;
        }

        // 3. Skip whitespace #2.
        while (p->x() > 0 && std::isspace(charAt(p)))
            p->decX();

        // 4. Skip non-whitespace.
        while (p->x() > 0 && !std::isspace(charAt(p)))
            p->decX();

        // 5. Move one step back if whitespace.
        if (std::isspace(charAt(p)))
            p->incX();

        return p;
        break;
    }
    default:
        return nullptr;
    }
}

std::size_t Buffer::size() const
{
    return txt_.size();
}

bool Buffer::atLnEnd(
    const PointPtr& point) const
{
    PointPtr p = point_;
    if (point)
        p = point;

    if (p->y() == size())
        return p->x() == lineLen() - 1;
    else 
        return p->x() == lineLen() - 2;
}

std::size_t Buffer::lineLen(
    const PointPtr& point) const
{
    if (!point)
        return txt_.at(point_->y()).size();
    else
        return txt_.at(point->y()).size();
}

std::string Buffer::getLine(
    const PointPtr& start) const
{
    if (!start)
        return txt_.at(point_->y());
    else
        return txt_.at(start->y()).substr(start->x());
}

char Buffer::charAt(
    const PointPtr& point) const
{
    if (!point)
        return txt_.at(point_->y()).at(point_->x());
    else
    {
        if (static_cast<int>(lineLen(point)) - 1 < point->x())
            return 0;
        else
            return txt_.at(point->y()).at(point->x());
    }
}

// SEURAAVAKS: KUN MVP TEHTY: PERU RANGE/POINTR pointtereist reffiin takas!!
// JA USEAMMIN OTA VAAN COPY! EI HALUTA EDITOIDA ALKUPERASTA
void Buffer::erase(
    const RangePtr& range)
{
    RangePtr rTmp = Range::make(range); 
    auto startX = rTmp->start()->x();
    auto endX = rTmp->end()->x();

    bool is = *rTmp->start() > *rTmp->end();

    if (is)
        rTmp->sortRange();

    startX = rTmp->start()->x();
    auto startY = rTmp->start()->y();
    endX = rTmp->end()->x();
    auto endY = rTmp->end()->y();

    if (rTmp->empty())
        return;

    if (rTmp->lines() == 1)
    {
        auto startX = rTmp->start()->x();
        auto startY = rTmp->start()->y();
        auto endX = rTmp->end()->x();
        auto endY = rTmp->end()->y();

        auto dif = endX - startX;

        txt_.at(rTmp->start()->y()).erase(
            rTmp->start()->x(),
            dif);
    }
    else if (rTmp->lines() == 2)
    {
        if (rTmp->start()->x() < 
            lineLen(rTmp->start()) && rTmp->start()->x() >= 0)
                txt_.at(rTmp->start()->y()).erase(rTmp->start()->x());

        if (rTmp->end()->x() > 0 && rTmp->end()->x() <= lineLen(rTmp->end()))
            txt_.at(rTmp->end()->y()).erase(0, rTmp->end()->x());
    }
    else if (rTmp->lines() > 2)
        txt_.erase(
            txt_.begin() + rTmp->start()->y() + 1,
            txt_.begin() + rTmp->end()->y());

    // Join lines if '\n' was erased.
    if (rTmp->lines() > 1)
    {
        txt_.at(startY).append(txt_.at(endY));
        txt_.erase(txt_.begin() + endY);
    }

}

const PointPtr& Buffer::cursor() const
{
    return point_;
}

const PointPtr& Buffer::setCursor(
    const PointPtr& newPos)
{
    if (!newPos)
        return point_;
    auto lastY = size();
    auto lastX = txt_.at(newPos->y()).size();

    if (newPos->y() != point_->y())
        if (newPos->y() >= 0 && newPos->y() <= lastY)
           point_->setY(newPos->y());

    if (newPos->x() != point_->x())
    {
        if (newPos->x() > lastX)
            point_->setX(lastX);
        else if (newPos->x() >= 0)
            point_->setX(newPos->x());
    }

    return point_;
}

//TODO: oke tee alkuu vaa reversefind oma funkkari.
RangePtr Buffer::find(
    std::regex& regex,
    const RangePtr& range) const
{
    if (!range->start()->isFullySet())
        range->start() |= point_;
    if (!range->end()->isFullySet())
        range->end()->set(point_->x(), size() - 1);

    for (int y = range->start()->y(); y <= range->end()->y(); ++y)
    {
        std::smatch match;
        std::string ln = txt_.at(y);

        int start = (y == range->start()->y()) ? range->start()->x() : 0;        
        int end = (y == range->end()->y()) ? range->end()->x() : ln.size();

        end = std::min(end, static_cast<int>(ln.size()));

        std::regex_search(
            ln.cbegin() + start, ln.cbegin() + end, match, regex);

        if (!match.empty())
            return Range::make(
                Point::make(match.position(0) + start, y),
                Point::make(
                    match.position(0) + start + match.length(0) - 1,
                    y));
    }

    return nullptr;
}

PointPtr Buffer::findCh(
    const char ch,
    const Direction dir,
    const PointPtr& point) const
{
    if (dir == Direction::UP || dir == Direction::DOWN)
        return nullptr;

    PointPtr p = point;

    if (!point)
        p = Point::make(point_);

    // Find in current line.
    auto ln = txt_.at(p->y());
    size_t pos = std::string::npos;

    if (dir == Direction::RIGHT && !atLnEnd(p))
        pos = ln.find(ch, p->x() + 1);
    else if (dir == Direction::LEFT && (p->x() != 0))
        pos = ln.rfind(ch, p->x() - 1);

    if (pos != std::string::npos)
        p->setX(pos);
    else
    {
        // Search rest of visible range.
        auto e = c_->visibleRange->end()->y();
        
        if (dir == Direction::RIGHT)
        {
            for (int i = p->y(); i < c_->visibleRange->end()->y() + 1; ++i)
            {
                ln = txt_.at(i);
                if (i == cursor()->y())
                    if (atLnEnd(p))
                        continue;
                    else
                        pos = ln.find(ch, p->x() + 1);
                else
                    pos = ln.find(ch);

                if (pos != std::string::npos)
                {
                    p->setX(pos);
                    p->setY(i);
                    break;
                }
            }
        }
        else if (dir == Direction::LEFT)
        {
            auto s = c_->visibleRange->start()->y();
            for (int i = p->y(); i >= c_->visibleRange->start()->y(); --i)
            {
                ln = txt_.at(i);
                if (i == cursor()->y())
                    if (!p->x())
                        continue;
                    else
                        pos = ln.rfind(ch, p->x() - 1);
                else
                    pos = ln.rfind(ch);

                if (pos != std::string::npos)
                {
                    p->setX(pos);
                    p->setY(i);
                    break;
                }
            }
        }
        else
            return nullptr;
    }

    if (pos == std::string::npos)
        return nullptr;
   else
        return p; 
}

}
