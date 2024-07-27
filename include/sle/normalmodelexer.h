#ifndef SLE_NORMALMODELEXER_H
#define SLE_NORMALMODELEXER_H

#if !defined(yyFlexLexerOnce)
#define yyFlexLexer nmFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

namespace sle
{

enum CmdType
{
    ATEOF,
    INSERT,
    APPEND,
    DELCHAR,
    WORDFWD,
    WORDBCK,
    TILLFWD,
    TILLBCK,
    TOFWD,
    TOBCK,
    QUIT,
};

class NormalModeLexer
    : public nmFlexLexer
{
public:
    NormalModeLexer(
        std::istream* arg_yyin = nullptr,
        std::ostream* arg_yyout = nullptr)
        :
        nmFlexLexer(arg_yyin, arg_yyout)
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

#endif // SLE_NORMALMODELEXER_H
