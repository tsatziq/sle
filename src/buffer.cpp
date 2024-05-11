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
        txt_.insert(txt_.begin() + point_.y() + 1, std::string());
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

    std::string& firstLn = newVec.front();
    std::string& lastLn = newVec.back();

    if (firstLn.length() >= range.start().x()) 
        firstLn = firstLn.substr(range.start().x());

    if (lastLn.length() >= range.end().x()) 
        lastLn = lastLn.substr(range.end().x());

   return newVec;
}

void Buffer::move(
    const Direction dir,
    const int count)
{
    // vai laittaako, et tas funk sisalla siirretaa scr cursor jos tarvis.
}

}
