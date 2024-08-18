#ifndef SLE_POINT_H
#define SLE_POINT_H

#include <memory>

namespace sle
{

enum class Axis
{
    NONE,
    X,
    Y,
};

class Point;
using PointPtr = std::shared_ptr<Point>;

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

    bool isXSet() const
    {
        return xSet_;
    }

    bool isYSet() const
    {
        return ySet_;
    }

    bool isFullySet() const
    {
        return xSet_ && ySet_;
    }

    bool isUnset() const
    {
        return !xSet_ && !ySet_;
    }

    void setXBool(
        const bool isSet)
    {
        xSet_ = isSet;
    }

    void setYBool(
        const bool isSet)
    {
        ySet_ = isSet;
    }

    static std::shared_ptr<Point> make(
        const int x,
        const int y)
    {
        return std::make_shared<Point>(x, y);
    }

    static std::shared_ptr<Point> make(
        const std::shared_ptr<Point>& point)
    {
        return std::make_shared<Point>(point->x(), point->y());
    }

    Point& operator|=(
        const Point& rhs)
    {
        if (!xSet_ && rhs.xSet_) { x_ = rhs.x_; xSet_ = rhs.xSet_; }
        if (!ySet_ && rhs.ySet_) { y_ = rhs.y_; ySet_ = rhs.ySet_; }
        return *this;
    }

private:
    int x_ = 0;
    int y_ = 0;
    bool xSet_ = false;
    bool ySet_ = false;
};

inline const PointPtr& operator|=(
    const PointPtr& lhs,
    const PointPtr& rhs)
{
    if (lhs && rhs)
        *lhs |= *rhs;

    return lhs;
}

class Range
{
public:
    Range() = default;

    Range(
        const PointPtr& start,
        const PointPtr& end)
        :
        start_(start),
        end_(end)
    {
        sortRange();
    }

    // Ensure start of range is lower than the end.
    void sortRange()
    {
        if (start_->y() < 0)
            start_->setY(0);
        if (start_->x() < 0)
            start_->setX(0);
        if (end_->y() < 0)
            end_->setY(0);
        if (end_->x() < 0)
            end_->setX(0);

        if (start_->y() > end_->y())
        {
            auto tmp = Point::make(start_);
            start_ = end_;
            end_ = tmp;
        }

        if ((start_->y() == end_->y())
            && (start_->x() > end_->x()))
        {
            int tmp = start_->x();
            start_->setX(end_->x());
            end_->setX(tmp);
        }
    }

    PointPtr start() 
    {
        return start_;
    }

    PointPtr end()
    {
        return end_;
    }

    void setRange(
        const PointPtr& start,
        const PointPtr& end)
    {
        start_ = start;
        end_ = end;
        sortRange();
    }

    void setStart(
        const PointPtr& start)
    {
        start_ = start;
    }

    void setEnd(
        const PointPtr& end)
    {
        end_ = end;
    }

    void fitToSize(
        const int lines,
        const int firstLnLen,
        const int lastLnLen)
    {
        int lastIndex = lines - 1;

        if (end_->y() > lastIndex)
            end_->setY(lastIndex);
        if (start_->y() > lastIndex)
            return;
    
        if (!firstLnLen)
            start_->setX(0);
        else if (start_->x() > firstLnLen - 1)
            start_->setX(firstLnLen - 1);
        if (!lastLnLen)
            end_->setX(0);
        else if (end_->x() > lastLnLen - 1)
            end_->setX(lastLnLen - 1);
    }

    std::size_t lines() const
    {
        return end_->y() - start_->y() + 1;
    }

    bool empty() const
    {
        return (start_->y() == end_->y()) && (start_->x() == end_->x());
    }

    bool contains(
        const PointPtr& point) const
    {
        return !(point->y() < start_->y() || point->y() > end_->y());
    }

    static std::shared_ptr<Range> make(
        const PointPtr& start,
        const PointPtr& end)
    {
        return std::make_shared<Range>(start, end);
    } 

    static std::shared_ptr<Range> make(
        const Point& start,
        const Point& end)
    {
        return std::make_shared<Range>(
            std::make_shared<Point>(start.x(), start.y()),
            std::make_shared<Point>(end.x(), end.y()));
    } 

    static std::shared_ptr<Range> make(
        const Point start,
        const Point end)
    {
        return std::make_shared<Range>(
            std::make_shared<Point>(start.x(), start.y()),
            std::make_shared<Point>(end.x(), end.y()));
    }

    static std::shared_ptr<Range> make(
        const PointPtr& start,
        const Point end)
    {
        return std::make_shared<Range>(
            start,
            std::make_shared<Point>(end.x(), end.y()));
    }
        
private:
    PointPtr start_;
    PointPtr end_;
};

using RangePtr = std::shared_ptr<Range>;

} // namespace tickingbot

#endif // SLE_POINT_H
