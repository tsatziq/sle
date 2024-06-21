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

    const Point& move(
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
        const RangePtr& range);

    const Point& cursor() const;

    /// Sets the cursor to specified point. 
    ///
    /// If the position is invalid, sets to the nearest valid position.
    /// \param newPos The new position of the cursor.
    /// \returns The new position of the cursor.
    const Point& setCursor(
        Point newPos);

    // TODO: muuta toi point ja ehk ranga smart ptr!!! tekee pahaa passata
    // non const valuena. mut valil pakko.

private:
    ContextPtr c_ = nullptr;
    std::vector<std::string> txt_ = { std::string() };
    Point point_ = Point(0, 0);
};

} // namespace sle

#endif // SLE_BUFFER_H
