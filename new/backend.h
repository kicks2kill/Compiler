#include <stdio.h>
#include "parser.h"

extern void Process(AST_Node *);

static void Code_gen_expression(Expression *expr) {
    switch( expr-> type) {
        case 'D':
            printf("PUSH %d\n", expr-> value);
            break;
        case 'P':
            Code_gen_expression(expr->left);
            Code_gen_expression(expr->right);
            switch(expr->oper) {
                case '+':
                    printf("ADD\n");
                    break;
                case '*':
                    printf("MULT\n");
                    break;
            }
        break;
    }
}
    
void Process(AST_Node *icode) {
    Code_gen_expression(icode);
    printf("PRINT\n");
}



static int Interpret_expression(Expression *expr) {
    switch(expr->type) {
        case 'D':
            return expr->value;
            break;
        case 'P':
            int e_left = Interpret_expression(expr->left);
            int e_right = Interpret_expression(expr->right);
            switch(expr->oper){
                case '+': return e_left + e_right;
                case '*': return e_left * e_right;
        }
        break;
    }
}
void Process(AST_Node *icode) {
    printf("%d\n", Interpret_expression(icode));
}
