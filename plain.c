/* Basic parser for now, will come back and refine later.*/

#include <stdio.h>
#include "lex.h"

/**
 * First thing expression does is call itself, the recursion never stops and the program doesn't terminate. 
 * At least until it runs out of stack space and is abnormally terminated by OS
*/
statements()
{
    expression();

    if( match( SEMI ))
        advance();
    else
        fprintf( stderr, "%d: Inserting missing semicolon\n", yylineno);
    
    if( !match( EOI ))
        statements(); //do another statement
}

expression()
{
    term();
    expr_prime();
}

expr_prime()
{
    if( match( PLUS ))
    {
        advance();
        term();
        expr_prime();
    }
     
}

term()
{
    factor();
    term_prime();
}

term_prime()
{
    if( match( TIMES ))
    {
        advance();
        factor();
        term_prime();
    }
}

factor()
{
    if( match( NUM_OR_ID ))
        advance();
    
    else if( match( LP ))
    {
        advance();
        expression();
        if ( match( RP ))
            advance();
        else
            fprintf( stderr, "%d: Mismatched parenthesis\n", yylineno);
    }
    else
         fprintf( stderr, "%d: Number or Identifier expected\n", yylineno);
}