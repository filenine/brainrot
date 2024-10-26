%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);

/* Define global variables if needed */
char *string_value;
extern int yylineno; 
%}

/* Define the union for different value types */
%union {
    int ival;
    char *sval;
}

/* Define token types */
%token PLUS MINUS TIMES DIVIDE SEMICOLON COMMA EQUALS
%token LPAREN RPAREN LBRACE RBRACE LT GT AND OR
%token BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM
%token EXTERN FLOAT FOR GOTO IF INT LONG REGISTER SHORT SIGNED
%token SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE
%token <sval> IDENTIFIER
%token <ival> NUMBER
%token <sval> STRING_LITERAL
%token RETURN

/* Define types for non-terminals if needed */
%type <ival> expression

/* Precedence rules */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right EQUALS
%left LT GT
%left PLUS MINUS
%left TIMES DIVIDE
%right UMINUS

%%
program:
    function
    ;

function:
    VOID IDENTIFIER LBRACE statements RBRACE
    ;

statements:
    /* empty */
    | statements statement
    ;

statement:
    declaration SEMICOLON
    | for_statement
    | return_statement SEMICOLON
    | expression SEMICOLON
    | print_statement SEMICOLON
    ;

declaration:
    INT IDENTIFIER
    | INT IDENTIFIER EQUALS expression
    ;

for_statement:
    FOR LPAREN init_expr SEMICOLON condition SEMICOLON increment RPAREN LBRACE statements RBRACE
    ;

init_expr:
    declaration
    | expression
    ;

condition:
    expression
    ;

increment:
    expression
    ;

print_statement:
    RETURN STRING_LITERAL    { printf("%s\n", $2); free($2); }
    ;

return_statement:
    RETURN expression    { printf("%d\n", $2); }
    ;

expression:
    NUMBER                        { $$ = $1; }
  | IDENTIFIER                    { $$ = 0; /* placeholder for variable lookup */ }
  | expression PLUS expression    { $$ = $1 + $3; }
  | expression MINUS expression   { $$ = $1 - $3; }
  | expression TIMES expression   { $$ = $1 * $3; }
  | expression DIVIDE expression  { $$ = $1 / $3; }
  | expression LT expression      { $$ = $1 < $3; }
  | MINUS expression %prec UMINUS { $$ = -$2; }
  | LPAREN expression RPAREN      { $$ = $2; }
  | IDENTIFIER EQUALS expression  { $$ = $3; /* Placeholder for assignment */ }
  ;

%%

int main(void) {
    return yyparse();
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}

