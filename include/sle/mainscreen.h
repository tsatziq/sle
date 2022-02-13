#ifndef SLE_MAINSCREEN_H
#define SLE_MAINSCREEN_H

#include "sle/screen.h"
#include "sle/dispatcher.h"

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
};
// FUNCTIONS:
// -show text that fits in screen
// -scrolling, next/prev page
//     -except these too are handled by others and then tehy just say PAINT
// 		-toisaalta kylla taa vois teha scrollin jos haluu, mut emt. ehk parempi ettei
// -remember height,width,number of lines, modes etc.
// possibly folding, wrapping, search, even completion?
// 
// NII enta jos oliski aina et koko muuttuu. lahtee eventti jos on uus max size,
// ja sit eri ikkunat ite muutas kokoaan? no mieti.. alus ainaki saa luua ittensa

} // namespace sle

#endif // SLE_MAINSCREEN_H
