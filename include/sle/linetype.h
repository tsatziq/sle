#ifndef SLE_LINETYPE_H
#define SLE_LINETYPE_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace sle {

class LineNum {
public:
    LineNum(std::string num);

    LineNum(unsigned num);

    static unsigned alphaToNum(const std::string numeral);

    std::string toStr();

    void operator++();

    LineNum& operator+=(const LineNum& num);

    friend LineNum operator+(LineNum lhs, const LineNum& rhs);

    LineNum& operator -=(const LineNum& num);

    friend LineNum operator-(LineNum lhs, const LineNum& rhs);

    void operator-(const LineNum& num);

    bool operator==(const LineNum& num) const;

    bool operator<(const LineNum& num) const;

private:
    unsigned val_;
    static const std::map<char, unsigned> charNumMap_;
    static const std::string chars_;
    static std::map<unsigned, std::string> numeralMap_;
};

} // namespace sle

#endif // SLE_LINETYPE_H

