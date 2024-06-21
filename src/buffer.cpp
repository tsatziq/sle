#include "sle/buffer.h"
#include <string_view>

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
    if (point_.y() < 0)
        return;
    if (point_.y() > txt_.size() - 1)
        return;

    txt_.at(point_.y()).insert(point_.x(), 1, ch);

    if (ch != '\n')
        point_.incX();
    else
    {
        point_.incX();
        std::string cutText = getLine(point_);

        if (!cutText.empty())
        {
            //Range delRange{point_, Point(lineLen(point_) - 1, point_.y())};
            erase(Range::make(point_, Point(lineLen(point_) - 1, point_.y())));
        }

        txt_.insert(txt_.begin() + point_.y() + 1, cutText);
        point_.set(0, point_.y() + 1);
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

// joskus ehk noi mitka muuttaa rangea eri funkkariin,
// sit vois laittaa ton parametrin const ref tjsp
std::vector<std::string> Buffer::getRange(
    Range range)
{
    if (txt_.empty())
        return {};

    range.fitToSize(txt_.size(), txt_.front().size(), txt_.back().size());

    std::vector<std::string> newVec;
    
    std::copy(
        txt_.begin() + (range.start().y()),
        txt_.begin() + (range.end().y() + 1),
        std::back_inserter(newVec));

    if (newVec.empty())
        return {};

    if (range.start().isXSet())
    {
        std::string& firstLn = newVec.front();

        if (firstLn.length() >= range.start().x()) 
            firstLn = firstLn.substr(range.start().x());
    }

    if (range.end().isXSet())
    {
        std::string& lastLn = newVec.back();

        if (lastLn.length() >= range.end().x()) 
            lastLn = lastLn.substr(range.end().x());
    }

   return newVec;
}

const Point& Buffer::move(
    const Direction& dir,
    const unsigned count)
{
    switch (dir)
    {
    case Direction::LEFT:
        if (point_.x() > count)
           point_.setX(point_.x() - count); 
        else
           point_.setX(0);
        break;
    case Direction::RIGHT:
    {
        auto length = lineLen(point_);

        if ((point_.x() + count) > length - 2)
        {
            if (length <= 2)
                point_.setX(0);
            else if (point_.y() == size() - 1)
                point_.setX(length - 1);
            else point_.setX(length - 2);
        }
        else
            point_.setX(point_.x() + count);
        break;
    }
    case Direction::UP:
    {
        if (point_.y() > count)
            point_.setY(point_.y() - count);
        else
            point_.setY(0);

        auto nextLnLen = lineLen(point_);

        if (nextLnLen <= point_.x())
        {
            if (nextLnLen <= 2)
                point_.setX(0);
            else
                point_.setX(nextLnLen - 2);
        }
        break;
    }
    case Direction::DOWN:
    {
        if (point_.y() + count > size() - 1)
            point_.setY(size() - 1);
        else
            point_.setY(point_.y() + count);

        auto nextLnLen = lineLen(point_);

        if (nextLnLen <= point_.x())
        {
            if (nextLnLen <= 2)
                point_.setX(0);
            else if (point_.y() == size() - 1)
                point_.setX(nextLnLen - 1);
            else
                point_.setX(nextLnLen - 2);
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
    const Point& point) const
{
    return txt_.at(point.y()).size();
}

std::string Buffer::getLine(
    const Point& start) const
{
    return txt_.at(start.y()).substr(start.x());
}

void Buffer::erase(
    const RangePtr& range)
{
    // pitasko olla tan sijaan vaa assert noista? ja range cosntructoris
    // varmistaa et pienempi arvo on start? tai joku isValid?
    range->fitToSize(
        txt_.size(),
        txt_.at(range->start().y()).size(),
        txt_.at(range->end().y()).size());

    //if (range->empty())
     //   return;

    if (range->lines() == 1)
    {
        txt_.at(range->start().y()).erase(
            range->start().x(),
            range->end().x() - range->start().x());
    }
    else if (range->lines() == 2)
    {
        if (range->start().x() < 
            lineLen(range->start()) && range->start().x() >= 0)
                txt_.at(range->start().y()).erase(range->start().x());

        if (range->end().x() > 0 && range->end().x() <= lineLen(range->end()))
            txt_.at(range->end().y()).erase(0, range->end().x());
    }
    else if (range->lines() > 2)
        txt_.erase(
            txt_.begin() + range->start().y() + 1,
            txt_.begin() + range->end().y());
}

const Point& Buffer::cursor() const
{
    return point_;
}

const Point& Buffer::setCursor(
    Point newPos)
{
    auto lastY = size();
    auto lastX = txt_.at(newPos.y()).size();

    if (newPos.y() != point_.y())
        if (newPos.y() > 0 && newPos.y() <= lastY)
           point_.setY(newPos.y());

    if (newPos.x() != point_.x())
    {
        if (newPos.x() > lastX)
            point_.setX(lastX);
        else if (newPos.x() > 0)
            point_.setX(newPos.x());
    }

    return point_;
}

}
