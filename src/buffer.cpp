#include "sle/buffer.h"
#include "sle/context.h"
#include "sle/linetype.h"
#include "sle/screen.h"
#include "sle/screenmanager.h"
#include "sle/types.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

namespace sle {

class BufferImpl : public Buffer
{
public:
    static std::unique_ptr<BufferImpl> create(const Context* context)
    {
        return std::unique_ptr<BufferImpl>(new BufferImpl(context));
    }

   ~BufferImpl();
   
    void readFile(const std::string& path) override;

    void saveFile(const std::string& path) override;

    void show() const override;

    int getSize() const override;

    const Text* getData() const override;

    void clear() override;

    void addLines(const Text& strs) override;

    void addChar(const char c) override;

    void eraseChar(const int num) override;

private:
    BufferImpl(const Context* context);

    const Context* c_;
    std::vector<std::string> lines_{""};
    Coord cursor_{0, 0};
    int topVisibleLine_ = 1;
    bool modified_ = false;
    Screen* scr_ = nullptr;
};

BufferImpl::BufferImpl(const Context* context)
    : c_(context)
    , scr_(context->screens->getScreen(ScreenId::main))
{}


BufferPtr Buffer::create(const Context* context)
{
    return BufferImpl::create(context);
}

BufferImpl::~BufferImpl()
{}

void BufferImpl::readFile(const std::string& path)
{
    if (path.empty())
        return;

    std::ifstream file(path);
    std::string line;

    if (file.is_open()) {
        while (file) {
            std::getline(file, line);
            lines_.push_back(line + "\n");
        }
    }
    
    show();
}

void BufferImpl::saveFile(const std::string& path)
{}

void BufferImpl::show() const
{
    scr_->paint(lines_);
}

int BufferImpl::getSize() const
{
    return lines_.size();
}

const Text* BufferImpl::getData() const
{
    return &lines_;
}

void BufferImpl::clear()
{
    lines_.clear();
}

void BufferImpl::addLines(const Text& strs)
{
    for (const std::string& str : strs)
        lines_.push_back(str);
}

void BufferImpl::addChar(const char c)
{
    //lines_.front() += c;

    lines_.at(cursor_.y).insert(cursor_.x, 1, c);
    if (c == '\n')
        lines_.push_back("");
}

void BufferImpl::eraseChar(const int num)
{
    if (cursor_.x + num < 0) {
        // if first line, do nothing
        if (cursor_.y == 0)
            return;

        cursor_.x = lines_.at(cursor_.y - 1).size() - 1;
        lines_.at(cursor_.y - 1).erase(cursor_.x, 1);

        // if erased line is not empty, join to previous line
        if (lines_.at(cursor_.y) != "\n" && !lines_.at(cursor_.y).empty())
            lines_.at(cursor_.y - 1) += lines_.at(cursor_.y);

        lines_.erase(lines_.begin() + cursor_.y);

        cursor_.y -= 1;
    } else {
        cursor_.x += num;
        lines_.at(cursor_.y).erase(cursor_.x, num);
    }
}

}
