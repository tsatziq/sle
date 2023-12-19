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

std::vector<std::string> Buffer::getRange(
    const Range& range) const
{
    std::vector<std::string> newVec;
    std::string firstLn = txt_.at(range.start().y());
    newVec.push_back(firstLn.substr(range.start().x()));
    
    std::copy(
        txt_.begin() + (range.start().y() + 1),
        txt_.begin() + (range.end().y() - 1),
        std::back_inserter(newVec));

   std::string lastLn = txt_.at(range.end().y());
   newVec.push_back(lastLn.substr(range.end().x()));
   
   return newVec;
}

void Buffer::move(
    const Direction dir,
    const int count)
{
    // vai laittaako, et tas funk sisalla siirretaa scr cursor jos tarvis.
}

}
