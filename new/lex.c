#include "lex.h"
#include "parser.h"


/**
 * For token clsses that contain only one token which is also an ASCII character ('+' for example), the class is the ASCII value of the char itself.
 * These numbers over 255 are chosen to avoid collisions with any ASCII values of single chars.
 * 
 * The representation of a token has at least two important uses. First, it is processed in one or more phases after the parser
 * to produce semantic information; examples are a numberic value produced from an integer token and an identification in some form from an identifier token
 * 
 * The representation of a token is usally a string, implemented as a pointer. But in this compiler, all tokens are single characters.
 */

static int is_layout_char(int ch) {
    switch(ch) {
        case ' ': 
        case '\t':
        case '\n':
            return 1;
        default:
            return 0;
    }
}

Token_type Token;

void get_next_token(void) {
    int ch;

    //get non-layout char
    do {
        ch = getchar();
        if(ch < 0) {
            Token.class = EoF; 
            Token.repr = "#";
            return;
        }
    } while (Is_layout_char(ch));

    if('0' <= ch && ch <= '9') {
        Token.class = DIGIT;
    } else {
        Token.class = ch;
    }

    Token.repr = ch;
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