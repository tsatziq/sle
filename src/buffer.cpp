#include "sle/buffer.h"
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
    static std::unique_ptr<BufferImpl> create()
    {
        return std::unique_ptr<BufferImpl>(new BufferImpl());
    }

    ~BufferImpl();

    void readFile(const std::string& path) override;

    void saveFile(const std::string& path) override;

    void show(Screen* scr) const override;

    int getSize() const override;

    const Text& getData() const override;

    void clear() override;

    void addLines(const Text& strs) override;

    void addChar(const char c) override;

    void eraseChar(const int num) override;

    // move cursor to own class
    void moveCursor(const int col, const int lines) override;

    void backWord(const unsigned num) override;

    void setX(const int value) override;

    Coord getCursor() const override;
private:
    BufferImpl();

    bool isBasic(const char c);

    bool isSymbol(const char c);

    std::vector<std::string> lines_{""};
    Coord cursor_{0, 0};
    int topVisibleLine_;
    bool modified_;
};

BufferImpl::BufferImpl()
    : topVisibleLine_(1)
    , modified_(false)
{}

BufferPtr Buffer::create()
{
    return BufferImpl::create();
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
            lines_.push_back(line);
        }
    }
}

void BufferImpl::saveFile(const std::string& path)
{}

void BufferImpl::show(Screen* scr) const
{
    scr->paint(lines_);
}

int BufferImpl::getSize() const
{
    return lines_.size();
}

const Text& BufferImpl::getData() const
{
    return lines_;
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

void BufferImpl::moveCursor(const int col, const int lines)
{
    cursor_.x += col;
    cursor_.y += lines;
}

void BufferImpl::backWord(const unsigned num)
{
    std::string line = lines_.at(cursor_.y);
    std::string::iterator lit = line.begin() + cursor_.x;

    if (isBasic(*(lit - 1))) {
        do {
            lit--;
            if (!isBasic(*(lit - 1))) {
                cursor_.x = lit - line.begin();
                break;
            } else if (lit == line.begin()) {
                cursor_.x = 0;
                break;
            }
        }
        while (lit != line.begin());
    }
}

void BufferImpl::setX(const int value)
{
    cursor_.x = value;
}

Coord BufferImpl::getCursor() const
{
    return cursor_;
}

bool BufferImpl::isBasic(const char c)
{
    return std::isalnum(c) || c == '_';
}

bool BufferImpl::isSymbol(const char c)
{
    return std::ispunct(c) && c != '_';
}

}
