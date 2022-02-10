#ifndef SLE_SCREENMANAGER_H
#define SLE_SCREENMANAGER_H

#include "sle/dispatcher.h"
#include "sle/eventaction.h"
#include <ncurses.h>

namespace sle
{

// Olisiko niin, etta talla ei olisi muistissa yhtaan sita tekstia itsellaan,
// vaan se taa vaan maalaa naytolle jne, ja saa sen itse tekstin silta Data
// tjsp classilta. Niin etta kun tulee komento, Data tekee muutokset ja sitten
// sanoo naytolle etta maalaa nama naytolle, sitten naytot kutsuu refreshia.
class ScreenManager
{
public:
    ScreenManager();

    ~ScreenManager();

private:
    const EventActionContainer actions_;
    const DispatcherPtr dispatcher_;
    const std::vector<ScreenBasePtr> screens_;
};

} // namespace sle

#endif // SLE_SCREENMANAGER_H
