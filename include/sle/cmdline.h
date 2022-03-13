#ifndef SLE_CMDLINE_H
#define SLE_CMDLINE_H

#include "sle/screen.h"
#include "sle/dispatcher.h"
#include "sle/linetype.h"

namespace sle
{

class CmdLine :
    public ScreenBase
{
public:
	static ScreenPtr create(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize); 

    virtual ~CmdLine() = default;

    virtual void paint(
        const StrPacket&  text) = 0;

    virtual void paint(const char c) = 0;

    virtual char getChar() const = 0;

    virtual std::string getLine() const = 0;

    virtual CursePtr getCurse() const = 0;
};

} // namespace sle

#endif // SLE_CMDLINE_H
