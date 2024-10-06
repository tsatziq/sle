#ifndef SLE_CMDMODELEXER_H
#define SLE_CMDMODELEXER_H

#if !defined(yyFlexLexerOnce)
#define yyFlexLexer cmFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

namespace sle
{

enum CmdType
{
    ATEOF,
    APPEND,
    EDIT,
    QUIT,
};

class CmdModeLexer
    : public cmFlexLexer
{
public:
    CmdModeLexer(
        std::istream* arg_yyin = nullptr,
        std::ostream* arg_yyout = nullptr)
        :
        cmFlexLexer(arg_yyin, arg_yyout)
    {}

    int yylex() override;

    void LexerError(
        const char* msg) override
    {
        error_ = true;  
    }

    bool hasError() const
    {
        return error_;
    }

private:
    bool error_ = false;
};

} // namespace sle

#endif // SLE_CMDMODELEXER_H
