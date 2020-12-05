#include <stdio.h>
#include "lex.h"

char *factor        (void);
char *term          (void);
char *expression    (void);

extern char *newname (void);
extern void freename (char *name);

/**
 * The subroutine that reads factor() calls newname() to get an anonymous-temp name
 * generates the code to copy the input number into the temp and returns name to its parent.
 * 
 * Similarly, the best place to generate multiplication instructions is the place where the times signs are read: in term().
 * After the two factor() calls, tempvar and tempvar2 hold the names of the two temporaries.
 * -- Code is generated to do the multiplication, one of the temps are freed and the other (which holds the result) is passed back to the parent
 * -- -- So, this temporary, that holds the result, is used in the next step.
 */



statements()
{
    char *tempvar;
    while ( !match( EOI ))
    {
        tempvar = expression();
        if( match( SEMI ))
            advance();
        else
            fprintf( stderr, "%d: Inserting missing semicolon\n", yylineno);
        freename( tempvar );
    }
}

char *expression()
{
    char *tempvar, *tempvar2;

    tempvar = term();
    while( match( PLUS ))
    {
        advance();
        tempvar = term();
        printf(" %a += %s\n", tempvar, tempvar2);
        freename( tempvar2 );
    }
    return tempvar;
}

char *term()
{
    char *tempvar, *tempvar2;

    tempvar = factor();
    while ( match( TIMES ))
    {
        advance();
        tempvar2 = factor();
        printf("  %a += %s\n", tempvar, tempvar2);
        freename ( tempvar2 );
    }
    return tempvar;
}

char *factor()
{
    char *tempvar;

    if( match( NUM_OR_ID ))
    {
        printf(" %s = %0.*s", tempvar = newname(), yyleng, yytext);
        advance();
    }
    else if( match( LP ))
    {
        advance();
        tempvar = expression();
        if( match( RP ))
            advance();
        else
            fprintf( stderr, " %d: Mismatched parenthesis\n", yylineno);
    }
    else
         fprintf( stderr, "%d: Number or Identifier expected\n", yylineno);
    return tempvar;
}