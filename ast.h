/* ast.h */

#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Forward declaration */
typedef struct ASTNode ASTNode;

typedef enum
{
    OP_PLUS,
    OP_MINUS,
    OP_TIMES,
    OP_DIVIDE,
    OP_MOD,
    OP_LT,
    OP_GT,
    OP_LE,
    OP_GE,
    OP_EQ,
    OP_NE,
    OP_AND,
    OP_OR,
    OP_NEG // For unary minus
    // Add other operators as needed
} OperatorType;

/* AST node types */
typedef enum
{
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_ASSIGNMENT,
    NODE_OPERATION,
    NODE_UNARY_OPERATION,
    NODE_FOR_STATEMENT,
    NODE_PRINT_STATEMENT,
    NODE_ERROR_STATEMENT,
    NODE_STATEMENT_LIST,
    NODE_IF_STATEMENT,
    NODE_STRING_LITERAL,
    NODE_SWITCH_STATEMENT,
    NODE_CASE,
    NODE_DEFAULT_CASE,
    NODE_BREAK_STATEMENT,
} NodeType;

/* Structures */
typedef struct StatementList
{
    ASTNode *statement;
    struct StatementList *next;
} StatementList;

typedef struct
{
    ASTNode *condition;
    ASTNode *then_branch;
    ASTNode *else_branch;
} IfStatementNode;

typedef struct CaseNode
{
    ASTNode *value;
    ASTNode *statements;
    struct CaseNode *next;
} CaseNode;

typedef struct
{
    ASTNode *expression; // The switch expression
    CaseNode *cases;     // Linked list of cases
} SwitchNode;

/* AST node structure */
struct ASTNode
{
    NodeType type;
    union
    {
        int value;  // For numbers
        char *name; // For identifiers
        struct
        { // For binary operations and assignments
            ASTNode *left;
            ASTNode *right;
            OperatorType op; // Operator character like '+', '-', etc.
        } op;
        struct
        { // For unary operations
            ASTNode *operand;
            char op; // Operator character like '-', etc.
        } unary;
        struct
        { // For 'for' statements
            ASTNode *init;
            ASTNode *cond;
            ASTNode *incr;
            ASTNode *body;
        } for_stmt;
        struct
        { // For print statements
            ASTNode *expr;
            bool newline;
        } print_stmt;
        StatementList *statements; // For statement lists
        IfStatementNode if_stmt;   // For if statements
        SwitchNode switch_stmt;
        CaseNode case_node;
        ASTNode *break_stmt; // For break statements, can be NULL
        // Add other nodes as needed
    } data;
};

/* Function prototypes */
ASTNode *create_number_node(int value);
ASTNode *create_identifier_node(char *name);
ASTNode *create_assignment_node(char *name, ASTNode *expr);
ASTNode *create_operation_node(OperatorType op, ASTNode *left, ASTNode *right);
ASTNode *create_unary_operation_node(OperatorType op, ASTNode *operand);
ASTNode *create_for_statement_node(ASTNode *init, ASTNode *cond, ASTNode *incr, ASTNode *body);
ASTNode *create_print_statement_node(ASTNode *expr, bool newline);
ASTNode *create_error_statement_node(ASTNode *expr);
ASTNode *create_statement_list(ASTNode *statement, ASTNode *next_statement);
ASTNode *create_if_statement_node(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch);
ASTNode *create_string_literal_node(char *string);
ASTNode *create_switch_statement_node(ASTNode *expression, CaseNode *cases);
CaseNode *create_case_node(ASTNode *value, ASTNode *statements);
CaseNode *create_default_case_node(ASTNode *statements);
CaseNode *append_case_list(CaseNode *list, CaseNode *case_node);
ASTNode *create_break_node();

int evaluate_expression(ASTNode *node);
void execute_statement(ASTNode *node);
void execute_statements(ASTNode *node);
void execute_for_statement(ASTNode *node);
void free_ast(ASTNode *node);

#endif /* AST_H */
