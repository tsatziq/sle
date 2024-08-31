#ifndef SLE_BUFFER_H
#define SLE_BUFFER_H

#include "sle/customtypes.h"
#include "sle/point.h"
#include "sle/context.h"
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

namespace sle
{

class Buffer
{
public:
    Buffer(
        const ContextPtr& context);

    void init();

    void addCh(
        const char ch,
        const bool replace = false);

    void addText(
        const std::string& txt);

    void addText(
        const std::vector<std::string>& txt);

    const std::vector<std::string>& getText();

    std::vector<std::string> getRange(
        const RangePtr& range);

    const PointPtr& move(
        const Direction& dir,
        const unsigned count = 1);

    const PointPtr& move(
        const Direction& dir,
        const int count) = delete;

    std::size_t size() const;

    /// Returns length of the line.
    std::size_t lineLen(
        const PointPtr& point = nullptr) const;

    // kato tarviiko tata loppujen lopuks, kunka usein kaytetaan.
    /// Returns rest of line from the specified point onwards.
    std::string getLine(
        const PointPtr& start) const;

    void erase(
        const RangePtr& range);

    const PointPtr& cursor() const;

    /// Sets the cursor to specified point. 
    ///
    /// If the position is invalid, sets to the nearest valid position.
    /// \param newPos The new position of the cursor.
    /// \returns The new position of the cursor.
    const PointPtr& setCursor(
        const PointPtr& newPos);

    /// Returns position of first regex match in string. Nullptr if not found.
    PointPtr find(
        std::regex& regex,
        const int y,
        const std::string& ln) const;

    /// Returns position of first regex match. Nullptr if not found.
    PointPtr find(
        std::regex& regex,
        const RangePtr& range = nullptr,
        const std::string& ln = {}) const;

    PointPtr findCh(
        const char ch,
        const Direction dir = Direction::RIGHT,
        const PointPtr& point = nullptr) const;

    // TODO: muuta toi point ja ehk ranga smart ptr!!! tekee pahaa passata
    // non const valuena. mut valil pakko.

private:
    ContextPtr c_ = nullptr;
    std::vector<std::string> txt_ = { std::string() };
    PointPtr point_ = Point::make(0, 0);
};

} // namespace sle

#endif // SLE_BUFFER_H
