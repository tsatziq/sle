#ifndef SLE_BUFFER_H
#define SLE_BUFFER_H

#include "sle/context.h"
#include <map>
#include <memory>

namespace sle
{

class Buffer
{
public:
    Buffer(
        const Context& context);

    void init();

private:
    Context c_;

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
