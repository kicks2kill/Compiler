#include <stdlib.h>
#include "lex.h"

typedef int Operator;
typedef struct _expression {
    char type;
    int value;
    struct _expression *left, *right;
    Operator oper;
} Expression;

typedef Expression AST_Node;

extern int Parse_program(AST_Node **);

static Expression *new_expression(void){
    return (Expression*)malloc( sizeof( Expression ));
}

static void free_expression(Expression *expr) { free(( void * )expr ); }
static int Parse_operator(Operator *oper_p);
static int Parse_expression(Expression **expr_p);

int Parse_program(AST_Node **icode_p) {
    Expression *expr;
    get_next_token();
    if( Parse_expression( &expr )) {
        if( Token.class != EoF) {
            Error("Garbage after end of Program");
        }
        *icode_p = expr;
        return 1;
    } 
    return 0;
}

int input(void){
  return expression() && require(token(EoF));
}

int expression(void){
  return term() && require(rest_expression());
}

int term(void) {
  return token(IDENTIFIER) || parenthesized_expression();
}

int parenthesized_expression(void){
  return token('(') && require(expression()) && require(token(')'));
}

int rest_expression(void){
  return token('+') && require(expression()) || 1;
}

int token(int tk){
  if(tk != Token.class) return 0;
  get_next_token(); return 1;
}
