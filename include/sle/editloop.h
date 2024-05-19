#ifndef SLE_EDITLOOP_H
#define SLE_EDITLOOP_H

#include "sle/context.h"
#include "sle/buffer.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace sle
{
// SEURAAVAKS: tee insert/cmd mode, ja pari liikkumiskomentoa.
// vois teha jotenki polymorfisesti, esim et tal on MODE class variablena.
// ja sit sil on virtuaalifunktio joku handleCmd tjsp, joka sit tehaa erilail
// riippuen modesta. sit siirra niit tarvittaes eri filuihin et ei tu liia isoa.
// kayta ObjectPool modeihin, ks ChatGPT!
// sit editloopin init() laita mode_:n NormalMode.
// noi modet tarvii sit ainaki contextptrn, tai ehk editloop ptrn
enum class Mode
{
    NORMAL,
    INSERT,
};

class EditLoop
{
public:
    EditLoop(
        const ContextPtr& context);

    void init();

private:
    class ModeBase
    {
    public:
        virtual ~ModeBase() = default;

        /// Take input and process them.
        /// \return true, if program should quit.
        virtual bool handle() = 0;
        // huom korvaa joskus bool enumilla.
    };

    using ModePtr = std::shared_ptr<ModeBase>;

    void changeMode(
        const Mode& mode);

    class NormalMode;
    class InsertMode;

    ContextPtr c_ = nullptr;
    ModePtr mode_ = nullptr;
    std::unordered_map<Mode, ModePtr> modePool_;
};

} // namespace sle

#endif // SLE_EDITLOOP_H
