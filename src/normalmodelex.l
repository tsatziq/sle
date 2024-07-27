%option c++ prefix="nm" yyclass="sle::NormalModeLexer"
%option stack noyywrap yylineno 8bit

%{

#include "sle/normalmodelexer.h"
#include <iostream>

using namespace sle;

%}

DIGIT      [0-9]
INT        {DIGIT}+

%%

i { return CmdType::INSERT; }
q { return CmdType::QUIT; }
<<EOF>> { return CmdType::ATEOF; }

.                  {
                       NormalModeLexer::LexerError("Invalid command");
                       return 0;
                   }

%%
