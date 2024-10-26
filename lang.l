%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lang.tab.h"

extern int yylineno;
%}

%option yylineno

%%

"skibidi"        { return VOID; }
"bruh"           { return BREAK; }
"sigma"          { return CASE; }
"yap"            { return CHAR; }
"grimace"        { return CONST; }
"grind"          { return CONTINUE; }
"based"          { return DEFAULT; }
"mewing"         { return DO; }
"gigachad"       { return DOUBLE; }
"amogus"         { return ELSE; }
"gyatt"          { return ENUM; }
"whopper"        { return EXTERN; }
"chad"           { return FLOAT; }
"flex"           { return FOR; }
"cringe"         { return GOTO; }
"edging"         { return IF; }
"rizz"           { return INT; }
"giga"           { return LONG; }
"edgy"           { return REGISTER; }
"bussin"         { return RETURN; }
"soy"            { return SHORT; }
"nut"            { return SIGNED; }
"maxxing"        { return SIZEOF; }
"salty"          { return STATIC; }
"gang"           { return STRUCT; }
"ohio"           { return SWITCH; }
"chungus"        { return UNION; }
"nonut"          { return UNSIGNED; }
"volatile"       { return VOLATILE; }
"goon"           { return WHILE; }
"main"           { return IDENTIFIER; }

"+"              { return PLUS; }
"-"              { return MINUS; }
"*"              { return TIMES; }
"/"              { return DIVIDE; }
";"              { return SEMICOLON; }
","              { return COMMA; }
"="              { return EQUALS; }
"("              { return LPAREN; }
")"              { return RPAREN; }
"{"              { return LBRACE; }
"}"              { return RBRACE; }
"<"              { return LT; }
">"              { return GT; }
"&&"             { return AND; }
"||"             { return OR; }

[0-9]+           { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
\"([^\\\"]|\\.)*\" {
    yylval.sval = strdup(yytext); 
    return STRING_LITERAL;
}
[ \t\n]+         ; /* Ignore whitespace */
.                { return *yytext; }

%%

int yywrap(void) {
    return 1;
}
