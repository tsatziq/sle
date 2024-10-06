#ifndef SLE_DEBUG_H
#degine SLE_DEBUG_H

#include <iostream>
#include <fstream>

namespace sle
{

struct flush_t
{} flush;

static constexpr std::string debugFile("build/debug");

class Debugger
{
public:
    Debugger()
    {
        file_.open(debugFile, std::ios::app);
    }
        
   ~Debugger()
    {
        if (file_.is_open())
            file_.close();
    } 

    template<typename T>
    Debugger& operator<<(
        const T& value)
    {
        if (file_.is_open())
            file_ << value;    
        return *this;
    }

    Debugger& operator<<(
        const flush_t&)
    {
        if (file_.is_open())
            file_.flush();
        return *this;
    }

private:
    std::ofstream file_;
};

}

} // namespace sle

#endif // SLE_DEBUG_H
