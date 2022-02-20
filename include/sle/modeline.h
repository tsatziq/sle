#ifndef SLE_MODELINE_H
#define SLE_MODELINE_H

#include "sle/screen.h"
#include "sle/dispatcher.h"

namespace sle
{

class ModeLine :
    public ScreenBase
{
public:
	static ScreenPtr create(
		const DispatcherPtr& dispatcher,
		const ScreenSize& maxSize); 

    virtual ~ModeLine() = default;

    virtual void paint(
        const StrPacket& text) = 0;
};

} // namespace sle

#endif // SLE_MODELINE_H
