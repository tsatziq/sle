#include "sle/cursor.h"
#include "sle/screenmanager.h"
#include "sle/types.h"
#include <memory>

namespace sle {

class CursorImpl : public Cursor
{
public:
    static std::unique_ptr<CursorImpl> create(
        const ScreenManager* scrs)
    {
        return std::unique_ptr<CursorImpl>(new CursorImpl(scrs));
    }

    ~CursorImpl();

    void move(const Coord to) override;

    Coord coord() const override;

private:
    CursorImpl(const ScreenManager* scrs);

    const ScreenManager* scrs_;
    Coord point_{0, 0, true, true};
};

CursorImpl::CursorImpl(const ScreenManager* scrs)
    : scrs_(scrs)
{}

CursorPtr Cursor::create(const ScreenManager* scrs)
{
    return CursorImpl::create(scrs);
}

CursorImpl::~CursorImpl()
{}

void CursorImpl::move(const Coord to)
{
    point_ = to;
    // call the curse function
}

Coord CursorImpl::coord() const
{
    return point_;
}

}

