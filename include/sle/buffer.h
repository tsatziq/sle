#ifndef SLE_BUFFER_H
#define SLE_BUFFER_H

#include "sle/customtypes.h"
#include "sle/point.h"
#include "sle/context.h"
#include <functional>
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

    /// Insert string to the buffer, to position indicated by point.
    ///
    /// \param point Where to insert string, if null, use cursor position.
    void addText(
        const std::string& txt,
        const PointPtr& point = nullptr);

    /// Appends vector of strings to the buffer.
    // JOSKUS: muuta nimi append ?
    void appendTxt(
        const std::vector<std::string>& txt);

    void insertTxt(
        const int index,
        const std::vector<std::string>& txt);

    const std::vector<std::string>& getText();

    std::vector<std::string> getRange(
        const RangePtr& range);

    const PointPtr& move(
        const Direction dir,
        const unsigned count = 1);

    const PointPtr& move(
        const Direction dir,
        const int count) = delete;

    PointPtr moveWord(
        const Direction dir,
        const PointPtr& point = nullptr,
        const unsigned count = 1);

    PointPtr moveWord(
        const Direction dir,
        const int count,
        const PointPtr& point) = delete;

    PointPtr moveWordEnd(
        const Direction dir,
        const PointPtr& point = nullptr); 

    std::size_t size() const;

    /// Returns length of the line.
    std::size_t lineLen(
        const PointPtr& point = nullptr) const;

    bool atLnEnd(
        const PointPtr& point = nullptr) const;

    // kato tarviiko tata loppujen lopuks, kunka usein kaytetaan.
    /// Returns rest of line from the specified point onwards.
    std::string getLine(
        const PointPtr& start = nullptr) const;

    char charAt(
        const PointPtr& point = nullptr) const;

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

    /// Returns position of first regex match. Nullptr if not found.
    RangePtr find(
        std::regex& regex,
        const RangePtr& range = nullptr) const;

    PointPtr findCh(
        const char ch,
        const Direction dir = Direction::RIGHT,
        const PointPtr& point = nullptr) const;

    /// Skips whitespace, if point not specified, uses current cursor position.
    ///
    /// \param f Function that returns bool type int.
    /// \param dir Direction to skip (left/right).
    /// \param point Point where to start. If not defined, use buffer cursor.
    /// \return Point where the cursor was left.
    PointPtr skip(
        int(*f)(int),
        const Direction dir,
        const PointPtr& point = nullptr);

private:
    ContextPtr c_ = nullptr;
    std::vector<std::string> txt_ = { "\n" };
    PointPtr point_ = Point::make(0, 0);
};

} // namespace sle

#endif // SLE_BUFFER_H
