%option c++ prefix="cm" yyclass="sle::CmdModeLexer"
%option stack noyywrap yylineno 8bit

%{

#include "sle/cmdmodelexer.h"
#include <iostream>

using namespace sle;

%}

DIGIT      [0-9]
INT        {DIGIT}+

%%

^e\h.*$ { return CmdType::EDIT; }
q { return CmdType::QUIT; }
<<EOF>> { return CmdType::ATEOF; }

.                  {
                       CmdModeLexer::LexerError("Invalid command");
                       return 0;
                   }

%%
