#ifndef SLE_MAINSCREEN_H
#define SLE_MAINSCREEN_H

#include "sle/screen.h"
#include "sle/dispatcher.h"
#include "sle/linetype.h"

namespace sle
{

struct PaintMainScr {
    StrPacket txt;
};

class MainScreen :
    public ScreenBase
{
public:
	static ScreenPtr create(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize); 

    virtual ~MainScreen() = default;

    virtual void paint(
        const StrPacket& text) = 0;

    virtual void paint(const char c) = 0;

    virtual char getChar() const = 0;

    virtual std::string getLine() const = 0;

    virtual CursePtr getCurse() const = 0;
};
// ok remove all this coe duplication, put these functions
// and CursePtr ONLY to ScreenBase

} // namespace sle

#endif // SLE_MAINSCREEN_H
