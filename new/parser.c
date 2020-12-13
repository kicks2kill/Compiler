#include "parser.h"

void input(void){
  switch(Token.class){
  case IDENTIFIER:
  case '(':
    expression();
    token(EoF);
    break;
  default:
    error();
  }
}

void expression(void) {
  switch(Token.class){
  case IDENTIFIER:
  case '(':
    term();
    rest_expression();
    break;
  default:
    error();
  }
}

void term(void){
  switch(Token.class) {
  case IDENTIFIER:
    token(IDENTIFIER);
    break;
  case '(':
    parenthesized_expression();
    break;
  default:
    error();
  }
}

void parenthesized_expression(void){
  switch(Token.class){
  case '(':
    token('(');
    expression();
    token(')');
    break;
  default:
    error();
  }
}

void rest_expression(void){
  switch(Token.class){
  case '+':
    token('+');
    expression();
    break;
  case EoF:
  case ')':
    break;
  default:
    error();
  }
}

void token(int tk){
  if( tk != Token.class) error();
  get_next_token();
}

static int Parse_operator(Operator *oper) {
    if(Token.class == '+') {
        *oper = '+';
        get_next_token();
        return 1;
    }
    if(Token.class == '*') {
        *oper = '*';
        get_next_token();
        return 1;
    }
    return 0;
}

static int Parse_expression(Expression **expr_p) {
    Expression *expr = *expr_p = new_expression();

    if(Token.class == DIGIT) {
        expr->type = 'D';
        expr->value = Token.repr - '0';
        get_next_token();
        return 1;
    }

    if(Token.class == '(') {
        expr->type = 'P';
        get_next_token();
        if( !Parse_expression(&expr->left)) {
            Error("Missing expression");
        }

        if(!Parse_operator(&expr->oper)) {
            Error("Missing opertor");
        }

        if(!Parse_expression(&expr->right)) {
            Error("Missing expression");
        }

        if(Token.class != ')') {
            Error("Missing ')' ");
        }
        get_next_token();
        return 1;
    }

    free_expression(expr);
    return 0;
}


