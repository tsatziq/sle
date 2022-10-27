#include "sle/buffer.h"
#include "sle/coord.h"
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

    void saveFile(const std::string& path = "") override;

    int size() const override;

    const Text* data() const override;

    void clear() override;

    void addLines(const Text& strs) override;

    void addChar(const char c) override;

    bool eraseChar(const int num) override;

    bool eraseLine(const int num) override;

private:
    BufferImpl(const Context* context);

    const Context* c_;
    std::vector<std::string> lines_{"\n"};
    bool modified_ = false;
    Screen* scr_ = nullptr;
    std::string filePath_{};
};

BufferImpl::BufferImpl(const Context* context)
    : c_(context)
    , scr_(context->screens->screen(ScreenId::main))
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

    filePath_ = path;
    std::ifstream file(path);
    std::string line;

    if (file.is_open())
    {
        lines_.clear();

        while (file)
        {
            std::getline(file, line);
            if (!line.empty())
                lines_.push_back(line + "\n");
        }
    }
}

void BufferImpl::saveFile(const std::string& path)
{
    if (path.empty() && filePath_.empty())
        return;
    else if (!path.empty())
        filePath_ = path;

    std::ofstream outFile(filePath_);

    if (outFile.is_open())
    {
        for (const auto& s : lines_)
            outFile << s;

        outFile.close();
    }
}

int BufferImpl::size() const
{
    return lines_.size();
}

const Text* BufferImpl::data() const
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
    Coord cursor = c_->cursor->coord();

    lines_.at(cursor.y()).insert(cursor.x(), 1, c);

    if (c == '\n')
    {
        auto sub = lines_.at(cursor.y()).substr(cursor.x() + 1);
        lines_.at(cursor.y()).erase(cursor.x() + 1);
        lines_.insert(lines_.begin() + cursor.y() + 1, sub);
    }
}

bool BufferImpl::eraseChar(const int num)
{
    Coord cursor = c_->cursor->coord();
    bool ret = true;

    //std::string& s = lines_.at(cursor.y());

    // Return if at beginning of line.
    if (cursor.x() == 0)
        ret = false;

    if (ret)
        lines_.at(cursor.y()).erase(cursor.x() - 1, 1);

    return ret;

/*
    if (cursor_.x() + num < 0) {
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
    */
}

bool BufferImpl::eraseLine(const int num)
{
    Coord cursor = c_->cursor->coord();
    bool ret = true;

    if (lines_.size() == 1 && lines_.at(0).empty())
        ret = false;
    else if (lines_.size() == 1 && !lines_.at(0).empty())
        lines_.at(0).clear();
    else
        lines_.erase(lines_.begin() + cursor.y());

    return ret;
}

}
