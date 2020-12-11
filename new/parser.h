#include <stdlib.h>
#include "lex.h"
#include "parser.h" 

static Expression *new_expression(void){
    return (Expression*)malloc( sizeof( Expression ));
}

static void free_expression(Expression *expr) { free(( void * )) expr ); }
static int Parse_operator(Operator *oper_p);
static int Parse_expression(Expression **expr_p);

int Parse_program(AST_node **icode_p) {
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