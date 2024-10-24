#ifndef SLE_SIDEBAR_H
#define SLE_SIDEBAR_H

#include "sle/screen.h"
#include "sle/dispatcher.h"
#include "sle/linetype.h"

namespace sle
{

struct AddToSideBar {
    const StrPacket lines;
};

class SideBar :
    public ScreenBase
{
public:
	static ScreenPtr create(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize); 

    virtual ~SideBar() = default;

    virtual void paint(
        const StrPacket& text) = 0;
};

} // namespace sle

#endif // SLE_SIDEBAR_H
