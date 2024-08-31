#ifndef SLE_INSERTMODE_H
#define SLE_INSERTMODE_H

#include "sle/editloop.h"
#include "sle/modebase.h"

namespace sle
{

class EditLoop::InsertMode
    : public ModeBase
{
    enum class InsertType
    {
        BASIC, ///< E.g. 'i' and 'a'.
        CHANGE, ///< E.g. 'c'.
        REPLACE, ///< 'R'.
    };

public:
    explicit InsertMode(
        EditLoop* parent);

    bool handle() override;

private:
    /// Supplementary information needed for different types of insert commands.
    struct InsertData
    {
        InsertType type = InsertType::BASIC; ///< Current insert command.
        PointPtr cEnd_ = Point::make(); ///< End point of change.
    };

    void quitMode();

    EditLoop* parent_ = nullptr;
    ContextPtr c_ = nullptr;
    InsertData data_;
};

} // namespace sle

#endif // SLE_INSERTMODE_H
