#include "sle/buffer.h"
#include <algorithm>
#include <iterator>
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
    const char ch)
{
    if (point_->y() < 0)
        return;
    if (point_->y() > txt_.size() - 1)
        return;

    txt_.at(point_->y()).insert(point_->x(), 1, ch);

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
    r.fitToSize(txt_.size(), txt_.front().size(), txt_.back().size());

    std::vector<std::string> newVec;
    
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

        if ((point_->x() + count) > length - 2)
        {
            if (length <= 2)
                point_->setX(0);
            else if (point_->y() == size() - 1)
                point_->setX(length - 1);
            else
                point_->setX(length - 2);
        }
        else
            point_->setX(point_->x() + count);
        break;
    }
    case Direction::UP:
    {
        if (point_->y() > count)
            point_->setY(point_->y() - count);
        else
            point_->setY(0);

        auto nextLnLen = lineLen(point_);

        if (nextLnLen <= point_->x())
        {
            if (nextLnLen <= 2)
                point_->setX(0);
            else
                point_->setX(nextLnLen - 2);
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

        if (nextLnLen <= point_->x())
        {
            if (nextLnLen <= 2)
                point_->setX(0);
            else if (point_->y() == size() - 1)
                point_->setX(nextLnLen - 1);
            else
                point_->setX(nextLnLen - 2);
        }
        break;
    }
    default:
        break;
    }

    return point_;
} 

std::size_t Buffer::size() const
{
    return txt_.size();
}

std::size_t Buffer::lineLen(
    const PointPtr& point) const
{
    return txt_.at(point->y()).size();
}

std::string Buffer::getLine(
    const PointPtr& start) const
{
    return txt_.at(start->y()).substr(start->x());
}

void Buffer::erase(
    const RangePtr& range)
{
    // pitasko olla tan sijaan vaa assert noista? ja range cosntructoris
    // varmistaa et pienempi arvo on start? tai joku isValid?
    auto sX = range->start()->x();
    auto sy = range->start()->y();
    auto eX = range->end()->x();
    auto eY = range->end()->y();

    /*
    range->fitToSize(
        txt_.size(),
        txt_.at(range->start()->y()).size(),
        txt_.at(range->end()->y()).size());
    */

    //if (range->empty())
     //   return;

    if (range->lines() == 1)
    {
        auto startX = range->start()->x();
        auto startY = range->start()->y();
        auto endX = range->end()->x();
        auto endY = range->end()->y();

        auto dif = endX - startX;

        txt_.at(range->start()->y()).erase(
            range->start()->x(),
            dif);
    }
    else if (range->lines() == 2)
    {
        if (range->start()->x() < 
            lineLen(range->start()) && range->start()->x() >= 0)
                txt_.at(range->start()->y()).erase(range->start()->x());

        if (range->end()->x() > 0 && range->end()->x() <= lineLen(range->end()))
            txt_.at(range->end()->y()).erase(0, range->end()->x());
    }
    else if (range->lines() > 2)
        txt_.erase(
            txt_.begin() + range->start()->y() + 1,
            txt_.begin() + range->end()->y());
}

const PointPtr& Buffer::cursor() const
{
    return point_;
}

const PointPtr& Buffer::setCursor(
    const PointPtr& newPos)
{
    auto lastY = size();
    auto lastX = txt_.at(newPos->y()).size();

    if (newPos->y() != point_->y())
        if (newPos->y() > 0 && newPos->y() <= lastY)
           point_->setY(newPos->y());

    if (newPos->x() != point_->x())
    {
        if (newPos->x() > lastX)
            point_->setX(lastX);
        else if (newPos->x() > 0)
            point_->setX(newPos->x());
    }

    return point_;
}

Point Buffer::find(
    const std::string& regex,
    const PointPtr& point) const
{
    // tee tasta se / find. eriksee simppelimpi f find.
    // ja se f tee vaa ettii nakyvasta rangesta.
    // kait kayta sit regex_search ja eti kaikki, ku iter ei toimi...
    /*    
    // tee se hemmetin PointPtr
    Point start = point;

    if (point->isUnset())
        start = point_;

    for (int i = start.y(); i < txt_.size(); ++i)
    {
        auto begin = std::regex_iterator<std::string::iterator>{
            txt_.at(i).begin(),
            txt_.at(i).end(),
            std::regex(regex)};

        auto end = std::sregex_iterator();
    } 
    */

    return Point{};
}

PointPtr Buffer::findCh(
    const char ch,
    const PointPtr& point) const
{
    PointPtr p = point;

    if (!point)
        p = Point::make(point_);

    // Find in current line.
    auto ln = txt_.at(p->y());
    auto res = std::find(ln.begin() + p->x(), ln.end(), ch);

    if (res != ln.end())
    {
        const auto pos = std::distance(ln.begin(), res);
        p->setX(pos);
    }
    else
    {
        // Search rest of visible range->
        for (int i = p->y() + 1; i < c_->visibleRange->end()->y(); ++i)
        {
            ln = txt_.at(i);
            res = std::find(ln.begin(), ln.end(), ch);
            if (res != ln.end())
            {
                const auto pos = std::distance(ln.begin(), res);
                p->setX(pos);
                p->setY(i);
            }
        }
    }

    if (res == ln.end())
        return nullptr;
    else
        return p; 
}

}
