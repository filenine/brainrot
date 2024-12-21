%{
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

int yylex(void);
void yyerror(const char *s);
void yapping(const char* format, ...);
void baka(const char* format, ...);

/* Symbol table to hold variable values */
typedef struct {
    char *name;
    int value;
} variable;

#define MAX_VARS 100
variable symbol_table[MAX_VARS];
int var_count = 0;

/* Function to add or update variables in the symbol table */
bool set_variable(char *name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            symbol_table[i].value = value;
            return true;
        }
    }
    if (var_count < MAX_VARS) {
        symbol_table[var_count].name = strdup(name);
        symbol_table[var_count].value = value;
        var_count++;
        return true;
    }
    return false;
}

/* Function to retrieve variable values */
int get_variable(char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].value;
        }
    }
    yyerror("Undefined variable");
    exit(1);
}

extern int yylineno;

/* Root of the AST */
ASTNode *root = NULL;
%}

%union {
    int ival;
    char *sval;
    ASTNode *node;
    CaseNode *case_node;
}

/* Define token types */
%token SKIBIDI RIZZ YAPPING YAPPIN BAKA MAIN BUSSIN FLEX 
%token PLUS MINUS TIMES DIVIDE MOD SEMICOLON COLON COMMA
%token LPAREN RPAREN LBRACE RBRACE
%token LT GT LE GE EQ NE EQUALS AND OR
%token BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM
%token EXTERN FLOAT FOR GOTO IF INT LONG REGISTER SHORT SIGNED
%token SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE
%token <sval> IDENTIFIER
%token <ival> NUMBER
%token <sval> STRING_LITERAL

/* Declare types for non-terminals */
%type <node> program skibidi_function
%type <node> statements statement
%type <node> declaration
%type <node> expression
%type <node> for_statement
%type <node> print_statement
%type <node> error_statement
%type <node> return_statement
%type <node> init_expr condition increment
%type <node> if_statement
%type <node> switch_statement break_statement
%type <case_node> case_list case_clause

%start program

/* Define precedence for operators */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right EQUALS           /* Assignment operator */
%left OR                /* Logical OR */
%left AND               /* Logical AND */
%nonassoc EQ NE         /* Equality operators */
%nonassoc LT GT LE GE   /* Relational operators */
%left PLUS MINUS        /* Addition and subtraction */
%left TIMES DIVIDE MOD  /* Multiplication, division, modulo */
%right UMINUS           /* Unary minus */

%%

program:
    skibidi_function
        { root = $1; }
    ;

skibidi_function:
    SKIBIDI MAIN LBRACE statements RBRACE
        { $$ = $4; }
    ;

statements:
      /* empty */
        { $$ = NULL; }
    | statements statement
        { $$ = create_statement_list($2, $1); }
    ;

statement:
      declaration SEMICOLON
        { $$ = $1; }
    | for_statement
        { $$ = $1; }
    | print_statement SEMICOLON
        { $$ = $1; }
    | error_statement SEMICOLON
        { $$ = $1; }
    | return_statement SEMICOLON
        { $$ = $1; }
    | if_statement
        { $$ = $1; }
    | switch_statement
        { $$ = $1; }
    | break_statement SEMICOLON
        { $$ = $1; }
    ;

switch_statement:
    SWITCH LPAREN expression RPAREN LBRACE case_list RBRACE
        { $$ = create_switch_statement_node($3, $6); }
    ;

case_list:
      /* empty */
        { $$ = NULL; }
    | case_list case_clause
        { $$ = append_case_list($1, $2); }
    ;

case_clause:
    CASE expression COLON statements
        { $$ = create_case_node($2, $4); }
    | DEFAULT COLON statements
        { $$ = create_default_case_node($3); }
    ;

break_statement:
    BREAK
        { $$ = create_break_node(); }
    ;  

if_statement:
      IF LPAREN expression RPAREN LBRACE statements RBRACE %prec LOWER_THAN_ELSE
        { $$ = create_if_statement_node($3, $6, NULL); }
    | IF LPAREN expression RPAREN LBRACE statements RBRACE ELSE if_statement %prec ELSE
        { $$ = create_if_statement_node($3, $6, $9); }
    | IF LPAREN expression RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE %prec ELSE
        { $$ = create_if_statement_node($3, $6, $10); }
    ;

declaration:
    optional_modifiers RIZZ IDENTIFIER
        { $$ = create_assignment_node($3, create_number_node(0)); }
    | optional_modifiers RIZZ IDENTIFIER EQUALS expression
        { $$ = create_assignment_node($3, $5); }
    ;

optional_modifiers:
      /* empty */
        { /* No action needed */ }
    | optional_modifiers modifier
        { /* No action needed */ }
    ;

modifier:
    VOLATILE
        { /* No action needed */ }
    ;

for_statement:
    FLEX LPAREN init_expr SEMICOLON condition SEMICOLON increment RPAREN LBRACE statements RBRACE
        {
            $$ = create_for_statement_node($3, $5, $7, $10);
        }
    ;

init_expr:
      declaration
        { $$ = $1; }
    | expression
        { $$ = $1; }
    ;

condition:
    expression
        { $$ = $1; }
    ;

increment:
    expression
        { $$ = $1; }
    ;

print_statement:
      YAPPING LPAREN expression RPAREN
        { $$ = create_print_statement_node($3, /*newline=*/true); }
    | YAPPIN LPAREN expression RPAREN
        { $$ = create_print_statement_node($3, /*newline=*/false); }
    ;

error_statement:
    BAKA LPAREN expression RPAREN
        { $$ = create_error_statement_node($3); }
    ;

return_statement:
    BUSSIN expression
        { $$ = $2; }
    ;

expression:
      NUMBER
        { $$ = create_number_node($1); }
    | IDENTIFIER
        { $$ = create_identifier_node($1); }
    | IDENTIFIER EQUALS expression
        { $$ = create_assignment_node($1, $3); }
    | expression PLUS expression
        { $$ = create_operation_node(OP_PLUS, $1, $3); }
    | expression MINUS expression
        { $$ = create_operation_node(OP_MINUS, $1, $3); }
    | expression TIMES expression
        { $$ = create_operation_node(OP_TIMES, $1, $3); }
    | expression DIVIDE expression
        { $$ = create_operation_node(OP_DIVIDE, $1, $3); }
    | expression MOD expression
        { $$ = create_operation_node(OP_MOD, $1, $3); }
    | expression LT expression
        { $$ = create_operation_node(OP_LT, $1, $3); }
    | expression GT expression
        { $$ = create_operation_node(OP_GT, $1, $3); }
    | expression LE expression
        { $$ = create_operation_node(OP_LE, $1, $3); }
    | expression GE expression
        { $$ = create_operation_node(OP_GE, $1, $3); }
    | expression EQ expression
        { $$ = create_operation_node(OP_EQ, $1, $3); }
    | expression NE expression
        { $$ = create_operation_node(OP_NE, $1, $3); }
    | expression AND expression
        { $$ = create_operation_node(OP_AND, $1, $3); }
    | expression OR expression
        { $$ = create_operation_node(OP_OR, $1, $3); }
    | MINUS expression %prec UMINUS
        { $$ = create_unary_operation_node(OP_NEG, $2); }
    | LPAREN expression RPAREN
        { $$ = $2; }
    | STRING_LITERAL
        { $$ = create_string_literal_node($1); }
    ;

%%

int main(void) {
    if (yyparse() == 0) {
        execute_statement(root);
    }
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}

void yapping(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void yappin(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void baka(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}