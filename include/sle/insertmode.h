#ifndef SLE_INSERTMODE_H
#define SLE_INSERTMODE_H

#include "sle/editloop.h"
#include "sle/modebase.h"
#include "sle/point.h"
#include <memory>

namespace sle
{

enum class InsertType
{
    BASIC, ///< E.g. 'i' and 'a'.
    CHANGE, ///< E.g. 'c'.
    REPLACE, ///< 'R'.
};

class InsertModeData
    : public ModeDataBase
{
public:
    InsertModeData() = default;

    InsertModeData(
        Action type,
        PointPtr from,
        PointPtr to)
        :
        type_(type),
        from_(from),
        to_(to)
    {}

    Action type() const
    {
        return type_;
    }

    void setType(
        const Action newType)
    {
        type_ = newType;
    }

    const PointPtr& from() const
    {
        return from_;
    }

    const PointPtr& to() const
    {
        return to_;
    }

private:
    Action type_ = Action::INSERT;
    PointPtr from_ = nullptr;
    PointPtr to_ = nullptr;
};

class InsertModeData;
using InsertDataPtr = std::shared_ptr<InsertModeData>;

class EditLoop::InsertMode
    : public ModeBase
{
public:
    explicit InsertMode(
        EditLoop* parent);

    bool handle() override;

    void setData(
        InsertModeData* data);

private:
    void quitMode();

    /// Erase characters from buffer and screen.
    void delBufScr(
        const RangePtr& range);

    EditLoop* parent_ = nullptr;
    ContextPtr c_ = nullptr;
    Buffer* buf_ = nullptr;
    MainScreen* scr_ = nullptr;
    InsertModeData data_;
};

} // namespace sle

#endif // SLE_INSERTMODE_H