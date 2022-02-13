#ifndef SLE_SCREENMANAGER_H
#define SLE_SCREENMANAGER_H

#include "sle/dispatcher.h"
#include "sle/screen.h"
#include <memory>

namespace sle
{

// Olisiko niin, etta talla ei olisi muistissa yhtaan sita tekstia itsellaan,
// vaan se taa vaan maalaa naytolle jne, ja saa sen itse tekstin silta Data
// tjsp classilta. Niin etta kun tulee komento, Data tekee muutokset ja sitten
// sanoo naytolle etta maalaa nama naytolle, sitten naytot kutsuu refreshia.
// ja screenmanager voi huolehtia esim kun pitaa muuttaa kokoa jne? vaikeeta.

// parempi varmaa nii, et taa on aika ohut classi. sit kun command pyytaa pienta
// ikkunaa esim appendille, se EventBase on lopulta joka luo sen ja lisaa tan listaan
// jne.

// data tekee muutoksen:
//     -sanoo et maalaa nama rivit ja sit screen tekee refresh
// lisaa pienen ruudun:
//     -luo se jokutyyppinen ruutu, se kutsuu managerin addScreen ja saa Id:n
//     -samoin deleteScreen
// window resize:
//     -kuuntele sita signaalia ja handlessa muuta kaikki
// tee muut:
// 	 	-ainaki alussa taa vois vaa antaa maxSize ja muut tekee siita ittensa,
// 		koska varsinki alussa kaikkien koot tieetaan jo.

enum : int
{
	SIDEBAR_WIDTH = 7,
	CMDLINE_INIT_HEIGHT = 1,
	MODELINE_HEIGHT = 1,
};

class ScreenManager;
using ScreenManagerPtr = std::shared_ptr<ScreenManager>;

class ScreenManager
{
public:
    static ScreenManagerPtr create(
        const DispatcherPtr& dispatcher);

    virtual ~ScreenManager() = default;

    virtual ScreenId addScreen(
        const Coord& startPoint) = 0;

    virtual void deleteScreen(
        const ScreenId screen) = 0;

protected:
    ScreenManager() = default;
};

} // namespace sle

#endif // SLE_SCREENMANAGER_H
