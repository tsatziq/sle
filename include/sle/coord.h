#ifndef SLE_COORD_H
#define SLE_COORD_H

#include <ostream>

namespace sle {

class Coord
{
public:
    Coord() {}

    Coord(int x, int y)
        : x_(x), y_(y), xSet_(true), ySet_(true)
    {}

    Coord(int x, int y, bool xSet, bool ySet)
        : x_(x), y_(y), xSet_(xSet), ySet_(ySet)
    {}

    bool isZero() const
    {
        return x_ == 0 && y_ == 0;
    }

    bool isUnset() const
    {
        return !xSet_ && !ySet_;
    }

    bool isComplete() const
    {
        return xSet_ && ySet_;
    }

    int x() const { return x_; }
    int y() const { return y_; }

    bool isXSet() const { return xSet_; }
    bool isYSet() const { return ySet_; }

    int setX(int newValue)
    {
        int oldValue = x_;
        x_ = newValue;
        xSet_ = true;
        return oldValue;
    }

    int setY(int newValue)
    {
        int oldValue = y_;
        y_ = newValue;
        ySet_ = true;
        return oldValue;
    }

    Coord operator+(const Coord& rhs) const
    {
        return Coord(x_ + rhs.x_
            , y_ + rhs.y_
            , xSet_ || rhs.xSet_
            , ySet_ || rhs.ySet_);
    }

    Coord operator-(const Coord& rhs) const
    {
        return Coord(x_ - rhs.x_
            , y_ - rhs.y_
            , xSet_ || rhs.xSet_
            , ySet_ || rhs.ySet_);
    }

    Coord operator-() const
    {
        return Coord(-x_, -y_, xSet_, ySet_);
    }

    Coord operator|(const Coord& rhs) const
    {
        return Coord(xSet_ ? x_ : rhs.x_
            , ySet_ ? y_ : rhs.y_
            , xSet_ || rhs.xSet_
            , ySet_ || rhs.ySet_);
    }

    Coord operator<<(const Coord& rhs) const
    {
        return Coord(rhs.xSet_ ? rhs.x_ : x_
            , rhs.ySet_ ? rhs.y_ : y_
            , xSet_ || rhs.xSet_
            , ySet_ || rhs.ySet_);
    }

    Coord& operator+=(const Coord& rhs)
    {
        if (rhs.xSet_) { x_ += rhs.x_; xSet_ = true; }
        if (rhs.ySet_) { y_ += rhs.y_; ySet_ = true; }
        return *this;
    }

    Coord& operator-=(const Coord& rhs)
    {
        if (rhs.xSet_) { x_ -= rhs.x_; xSet_ = true; }
        if (rhs.ySet_) { y_ -= rhs.y_; ySet_ = true; }
        return *this;
    }

    Coord operator|=(const Coord& rhs)
    {
        if (!xSet_) { x_ = rhs.x_; xSet_ = rhs.xSet_; }
        if (!ySet_) { y_ = rhs.y_; ySet_ = rhs.ySet_; }
        return *this;
    }

    Coord operator<<=(const Coord& rhs)
    {
        if (rhs.xSet_) { x_ = rhs.x_; xSet_ = true; }
        if (rhs.ySet_) { y_ = rhs.y_; ySet_ = true; }
        return *this;
    }


    bool operator==(const Coord& rhs) const
    {
        return (x_ == rhs.x_) && (y_ == rhs.y_);
    }

    bool operator!=(const Coord& rhs) const
    {
        return !(*this == rhs);
    }

    Coord& operator*=(const int value)
    {
        if (xSet_) { x_ *= value; }
        if (ySet_) { y_ *= value; }
        return *this;
    }

    std::string toString() const
    {
        std::string cStr = "(";

        cStr += (isXSet() ? std::to_string(x()) : "*") + ", ";
        cStr += (isYSet() ? std::to_string(y()) : "*") + ", ";

        return cStr;
    }

private:
    int x_ = 0;
    int y_ = 0;
    bool xSet_ = false;
    bool ySet_ = false;
};

inline std::ostream& operator<<(std::ostream& s, const Coord& c)
{
    return s << c.toString();
}

} // namespace sle

#endif // SLE_COORD_H
