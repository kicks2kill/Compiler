#include "parser.h"



int main(void) {
    AST_NODE *icode;

    if( !Parse_program(&icode)) Error("No top-level expression");
    Process(icode);

    return 0;
}