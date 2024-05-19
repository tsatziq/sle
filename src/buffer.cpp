#include "sle/buffer.h"

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
        Range delRange = Range(point_, Point(lineLen(point_) - 1, point_.y()));
        erase(delRange);


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

    std::string& firstLn = newVec.front();
    std::string& lastLn = newVec.back();

    if (firstLn.length() >= range.start().x()) 
        firstLn = firstLn.substr(range.start().x());

    if (lastLn.length() >= range.end().x()) 
        lastLn = lastLn.substr(range.end().x());

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
    Range& range)
{
    range.fitToSize(txt_.size(), txt_.at(range.start().y()).size(), txt_.at(range.end().y()).size());

    if (range.empty())
        return;

    if (range.lines() == 1)
        txt_.at(range.start().y()).erase(range.start().x(), range.end().x());
    else if (range.lines() == 2)
    {
        if (range.start().x() < lineLen(range.start()) && range.start().x() >= 0)
            txt_.at(range.start().y()).erase(range.start().x());

        if (range.end().x() > 0 && range.end().x() <= lineLen(range.end()))
            txt_.at(range.end().y()).erase(0, range.end().x());
    }
    else if (range.lines() > 2)
        txt_.erase(txt_.begin() + range.start().y() + 1, txt_.begin() + range.end().y());
}

}
