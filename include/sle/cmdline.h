#ifndef SLE_CMDLINE_H
#define SLE_CMDLINE_H

#include "sle/screen.h"
#include "sle/dispatcher.h"

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
};

} // namespace sle

#endif // SLE_CMDLINE_H
