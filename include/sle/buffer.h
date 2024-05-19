#ifndef SLE_BUFFER_H
#define SLE_BUFFER_H

#include "sle/point.h"
#include "sle/context.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace sle
{

enum class Direction
{
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

class Buffer
{
public:
    Buffer(
        const ContextPtr& context);

    void init();

    void addCh(
        const char ch);

    void addText(
        const std::string& txt);

    void addText(
        const std::vector<std::string>& txt);

    const std::vector<std::string>& getText();

    //korvaa toi vector string joskus sharedpointerilla
    std::vector<std::string> getRange(
        Range range);

    const Point&  move(
        const Direction& dir,
        const unsigned count = 1);

    const Point& move(
        const Direction& dir,
        const int count) = delete;

    std::size_t size() const;

    /// Returns length of the line.
    std::size_t lineLen(
        const Point& point) const;

    // kato tarviiko tata loppujen lopuks, kunka usein kaytetaan.
    /// Returns rest of line from the specified point onwards.
    std::string getLine(
        const Point& start) const;

    void erase(
        Range& range);

    // joku to from screen coord static funk?
    // ettei tartte muistaa eriksee screen ja buf kursoria
    // kattoo vaa ettei mee yli rajojen ja et mik' on ylin nakyva rivi.
    // screenille kans cursor? ja jonneki ehk isPointVisible() ja sit
    // noi convert funkkarit.
    // pitasko sittenki olla vaa yks screen ja yks buffer. sitte tehaa
    // serveri joka handlaa sen filujen vaihdon jne? ois ainaki simppeli
    // sais contextii kaiken :s
    // oisko toScreenPoint tjsp tos screen luokassa. ja isPointVisible()
    // muista kattoo et onko menny sivureunojen yli jne.
    // sitte toBufferPoint pitas olla helppo, mut tarkasta kuitneki et
    // se pointti ei ole tyhjaa tilaa.

private:
    ContextPtr c_ = nullptr;
    std::vector<std::string> txt_ = { std::string() };
    Point point_ = Point(0, 0);

    //void addToScreen(const LineNum from);
    /* 
    std::map<LineNum, std::string> lines_;
    LineNum topVisibleLine_;
    LineNum screenHeight_;
    bool modified_;
    */
};

} // namespace sle

#endif // SLE_BUFFER_H
