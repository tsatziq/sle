#include "sle/linetype.h"
#include <cctype>
#include <algorithm>

namespace sle {

constexpr unsigned maxLines = 180;

const std::map<char, unsigned> LineNum::charNumMap_ {
    {'A', 1}, {'B', 2}, {'C', 3}, {'D', 4}, {'E', 5}, {'F', 6}, {'G', 7}
    , {'H', 8}, {'I', 9}, {'J', 10}, {'K', 11}, {'L', 12}, {'M', 13}, {'N', 14}
    , {'O', 15}, {'P', 16}, {'Q', 17}, {'R', 18}, {'S', 19}, {'T', 20}
    , {'U', 21}, {'V', 22}, {'W', 23}, {'X', 24}, {'Y', 25}, {'Z', 26}
};

const std::string LineNum::chars_ {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

LineNum::LineNum(std::string num) : val_(alphaToNum(num))
{}

LineNum::LineNum(unsigned num) : val_(num)
{}

unsigned LineNum::alphaToNum(const std::string numeral)
{
    unsigned sum;

    // TODO: Check that numeral is alphabetic only
    
    switch (numeral.length()) {
    case 1:
        sum = charNumMap_.at(std::toupper(numeral[0]));
        break;
    case 2:
        sum = charNumMap_.at(std::toupper(numeral[0])) * 26;
        sum += charNumMap_.at(std::toupper(numeral[1]));
        break;
    default:
        // TODO: Send error for now, it would be over 700 lines.
        break;
    }

    return sum;
}

std::string LineNum::toStr()
{
    return numeralMap_.at(val_);
}

void LineNum::operator++()
{
    ++val_;
}

LineNum& LineNum::operator+=(const LineNum& num)
{
    val_ += num.val_;

    return *this;
}

LineNum operator+(LineNum lhs, const LineNum& rhs)
{
    lhs += rhs;

    return lhs;
}

LineNum& LineNum::operator-=(const LineNum& num)
{
    val_ -= num.val_;

    return *this;
}

LineNum operator-(LineNum lhs, const LineNum& rhs)
{
    lhs -= rhs;

    return lhs;
}

bool LineNum::operator==(const LineNum& num) const
{
    return val_ == num.val_;
}

bool LineNum::operator<(const LineNum& num) const
{
    return val_ < num.val_;
}

std::map<unsigned, std::string> LineNum::numeralMap_ = []
{
    std::map<unsigned, std::string> numMap;

    std::string numeral;

    int dividend, quotient, remainder, ogQuotient;
    
    for (unsigned i = 1; i <= maxLines; ++i) {

    // Need to calculate first digit separately
    dividend = i - 1;
    remainder = dividend % 26;
    quotient = dividend / 26;
    numeral = chars_[remainder];

    // Loop the rest digits if necessary
    if (i > 26) {
        do {
            ogQuotient = quotient;
			dividend = quotient - 1;
			remainder = dividend % 26; 
			quotient = dividend / 26;
			numeral = chars_[remainder] + numeral;
        } while (ogQuotient > 26);
    }

    numMap.insert({i, numeral});

    }
    
    return numMap;
}();

}

