#ifndef SLE_POINT_H
#define SLE_POINT_H

namespace sle
{

enum class Axis
{
    NONE,
    X,
    Y,
};

class Point
{
public:
    Point()
    {}

    Point(
        int xIn,
        int yIn)
        :
        x_(xIn),
        y_(yIn),
        xSet_(true),
        ySet_(true)
    {} 

    Point(
        int xIn,
        int yIn,
        bool xSetIn,
        bool ySetIn)
        :
        x_(xIn),
        y_(yIn),
        xSet_(xSetIn),
        ySet_(ySetIn)
    {}

    Point(
        const Axis axis,
        const int value)
    {
        switch (axis)
        {
        case Axis::X:
            x_ = value;
            xSet_ = true;
            break;
        case Axis::Y:
            y_ = value;
            ySet_ = true;
            break;
        case Axis::NONE:
        default:
            break;
        }
    }         

    int x() const
    {
        return x_;
    }

    int y() const
    {
        return y_;
    }

    void setX(
        const int x)
    {
        x_ = x;
    }

    void setY(
        const int y)
    {
        y_ = y;
    }

    void set(
        const int x,
        const int y)
    {
        x_ = x;
        y_ = y;
    }

    void incX()
    {
        ++x_;
    }

    void incY()
    {
        ++y_;
    }

    void decX()
    {
        --x_;
    }

    void decY()
    {
        --y_;
    }
        
private:
    int x_ = 0;
    int y_ = 0;
    bool xSet_ = false;
    bool ySet_ = false;
};

class Range
{
public:
    Range() = default;

    Range(
        const Point& start,
        const Point& end)
        :
        start_(start),
        end_(end)
        {}

    Point start() const
    {
        return start_;
    }

    Point end() const
    {
        return end_;
    }

    void setRange(
        const Point& start,
        const Point& end)
    {
        start_ = start;
        end_ = end;
    }

    void setStart(
        const Point& start)
    {
        start_ = start;
    }

    void setEnd(
        const Point& end)
    {
        end_ = end;
    }
        
private:
    Point start_;
    Point end_;
};

} // namespace tickingbot

#endif // SLE_POINT_H
