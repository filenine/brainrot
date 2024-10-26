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
    NODE_STATEMENT_LIST,
    NODE_IF_STATEMENT,
    NODE_STRING_LITERAL
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
        StatementList *statements; // For statement lists
        IfStatementNode if_stmt;   // For if statements
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
ASTNode *create_print_statement_node(ASTNode *expr);
ASTNode *create_statement_list(ASTNode *statement, ASTNode *next_statement);
ASTNode *create_if_statement_node(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch);
ASTNode *create_string_literal_node(char *string);
int evaluate_expression(ASTNode *node);
void execute_statement(ASTNode *node);
void execute_statements(ASTNode *node);
void execute_for_statement(ASTNode *node);
void free_ast(ASTNode *node);

#endif /* AST_H */
