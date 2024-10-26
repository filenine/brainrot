/* ast.c */

#include "ast.h"
#include <stdbool.h>

/* Include the symbol table functions */
extern bool set_variable(char *name, int value);
extern int get_variable(char *name);
extern void yyerror(const char *s);
extern void yapping(const char *format, ...);

/* Function implementations */

ASTNode *create_number_node(int value)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->data.value = value;
    return node;
}

ASTNode *create_identifier_node(char *name)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    node->data.name = strdup(name);
    return node;
}

ASTNode *create_assignment_node(char *name, ASTNode *expr)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGNMENT;
    node->data.op.left = create_identifier_node(name);
    node->data.op.right = expr;
    node->data.op.op = '=';
    return node;
}

ASTNode *create_operation_node(OperatorType op, ASTNode *left, ASTNode *right)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_OPERATION;
    node->data.op.left = left;
    node->data.op.right = right;
    node->data.op.op = op;
    return node;
}

ASTNode *create_unary_operation_node(OperatorType op, ASTNode *operand)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_UNARY_OPERATION;
    node->data.unary.operand = operand;
    node->data.unary.op = op;
    return node;
}

ASTNode *create_for_statement_node(ASTNode *init, ASTNode *cond, ASTNode *incr, ASTNode *body)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_FOR_STATEMENT;
    node->data.for_stmt.init = init;
    node->data.for_stmt.cond = cond;
    node->data.for_stmt.incr = incr;
    node->data.for_stmt.body = body;
    return node;
}

ASTNode *create_print_statement_node(ASTNode *expr)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_PRINT_STATEMENT;
    node->data.op.left = expr;
    return node;
}

ASTNode *create_statement_list(ASTNode *statement, ASTNode *next_statement)
{
    if (!next_statement)
    {
        /* Create a new statement list */
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = NODE_STATEMENT_LIST;
        node->data.statements = malloc(sizeof(StatementList));
        node->data.statements->statement = statement;
        node->data.statements->next = NULL;
        return node;
    }
    else
    {
        /* Prepend to existing list */
        StatementList *list = malloc(sizeof(StatementList));
        list->statement = statement;
        list->next = next_statement->data.statements;
        next_statement->data.statements = list;
        return next_statement;
    }
}

int evaluate_expression(ASTNode *node)
{
    if (!node)
        return 0;
    switch (node->type)
    {
    case NODE_NUMBER:
        return node->data.value;
    case NODE_IDENTIFIER:
        return get_variable(node->data.name);
    case NODE_ASSIGNMENT:
    {
        int value = evaluate_expression(node->data.op.right);
        set_variable(node->data.op.left->data.name, value);
        return value;
    }
    case NODE_OPERATION:
    {
        int left = evaluate_expression(node->data.op.left);
        int right = evaluate_expression(node->data.op.right);
        switch (node->data.op.op)
        {
        case OP_PLUS:
            return left + right;
        case OP_MINUS:
            return left - right;
        case OP_TIMES:
            return left * right;
        case OP_DIVIDE:
            return left / right;
        case OP_MOD:
            return left % right;
        case OP_LT:
            return left < right;
        case OP_GT:
            return left > right;
        case OP_LE:
            return left <= right;
        case OP_GE:
            return left >= right;
        case OP_EQ:
            return left == right;
        case OP_NE:
            return left != right;
        case OP_AND:
            return left && right;
        case OP_OR:
            return left || right;
        default:
            yyerror("Unknown operator");
            return 0;
        }
    }
    case NODE_UNARY_OPERATION:
    {
        int operand = evaluate_expression(node->data.unary.operand);
        switch (node->data.unary.op)
        {
        case OP_NEG:
            return -operand;
        default:
            yyerror("Unknown unary operator");
            return 0;
        }
    }
    case NODE_STRING_LITERAL:
        yyerror("Cannot evaluate a string literal as an integer");
        return 0;
    default:
        yyerror("Unknown expression type");
        return 0;
    }
}

void execute_statement(ASTNode *node)
{
    if (!node)
        return;
    switch (node->type)
    {
    case NODE_ASSIGNMENT:
    case NODE_OPERATION:
    case NODE_UNARY_OPERATION:
    case NODE_NUMBER:
    case NODE_IDENTIFIER:
        evaluate_expression(node);
        break;
    case NODE_FOR_STATEMENT:
        execute_for_statement(node);
        break;
    case NODE_PRINT_STATEMENT:
    {
        ASTNode *expr = node->data.op.left;
        if (expr->type == NODE_STRING_LITERAL)
        {
            yapping("%s\n", expr->data.name);
        }
        else
        {
            int value = evaluate_expression(expr);
            yapping("%d\n", value);
        }
        break;
    }
    case NODE_STATEMENT_LIST:
        execute_statements(node);
        break;
    case NODE_IF_STATEMENT:
        if (evaluate_expression(node->data.if_stmt.condition))
        {
            execute_statement(node->data.if_stmt.then_branch);
        }
        else if (node->data.if_stmt.else_branch)
        {
            execute_statement(node->data.if_stmt.else_branch);
        }
        break;
    default:
        yyerror("Unknown statement type");
        break;
    }
}

void execute_statements(ASTNode *node)
{
    if (!node)
        return;
    if (node->type != NODE_STATEMENT_LIST)
    {
        execute_statement(node);
        return;
    }
    StatementList *current = node->data.statements;
    /* Reverse the list to maintain correct order */
    StatementList *reversed = NULL;
    while (current)
    {
        StatementList *next = current->next;
        current->next = reversed;
        reversed = current;
        current = next;
    }
    current = reversed;
    while (current)
    {
        execute_statement(current->statement);
        current = current->next;
    }
}

void execute_for_statement(ASTNode *node)
{
    evaluate_expression(node->data.for_stmt.init);
    while (evaluate_expression(node->data.for_stmt.cond))
    {
        execute_statement(node->data.for_stmt.body);
        evaluate_expression(node->data.for_stmt.incr);
    }
}

ASTNode *create_if_statement_node(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_IF_STATEMENT;
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.then_branch = then_branch;
    node->data.if_stmt.else_branch = else_branch;
    return node;
}

ASTNode *create_string_literal_node(char *string)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_STRING_LITERAL;
    node->data.name = string;
    return node;
}
