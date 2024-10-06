#ifndef SLE_CMDLINE_H
#define SLE_CMDLINE_H

#include "sle/screen.h"
#include "sle/editloop.h"

namespace sle
{

class CmdLine
    : public Screen
{
public:
    explicit CmdLine(
        ContextPtr& context);

    ~CmdLine();

    void init();

    const PointPtr& cursor() const;

    void paintLn(
        const std::string str);

    bool paintCh(
        const char c,
        const bool replace = false);

    void moveCursor(
        const PointPtr& point);

    int getCh();

    void clear();

    void update();

private:
    ContextPtr c_ = nullptr;
    int width_;
    int height_;
    PointPtr cursor_ = Point::make(0, 0);
};

} // namespace sle

#endif // SLE_CMDLINE_H
