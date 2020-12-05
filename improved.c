/* Revised parser */

#include <stdio.h>
#include "lex.h"

void factor     (void);
void term       (void);
void expression (void);

/**
 * The naive parser "plain.c" is useful for explaining things, but not much in practice.
 * The main difficulty is the tremendous amount of unnecessary recursion.
 * If the last thing that a subroutine does is call itself, then that recursive call can be replaced by a loop. Right recursion is often called tail recursion
 * 
 * ==================== Why this is better ==========================
 * I've added a little error recovery by adding code to each subroutine that exmaines the current lookahead token before doing anything else.
 * It's an error if that token cannot legitimately occur in the input. For example, expressions all have to start with either a NUM_OR_ID or an LP... otherwise something is wrong
*/

statements()
{
    while( !match( EOI ))
    {
        expression();

        if( match( SEMI ))
            advance();
        else
            fprintf(stderr, "%d: Inserting missing semicolon\n", yylineno);
    }
}

void expression()
{
    if( !legal_lookahead( NUM_OR_ID, LP, 0 ))
        return;
    
    term();
    while( match( PLUS ))
    {
        advance();
        term();
    }
}

void term()
{
    if( !legal_lookahead( NUM_OR_ID, LP, 0 ))
        return;
    
    factor();
    while( match( TIMES ))
    {
        advance();
        factor();
    }
}

void factor()
{
    if( !legal_lookahead( NUM_OR_ID, LP, 0 ) )
        return;
    
    if( match( NUM_OR_ID ))
        advance();
    
    else if( match( LP ))
    {
        advance();
        expression();
        if( match( RP ))
            advance();
        else
            fprintf( stderr, "%d: Mismatched Parenthesis\n", yylineno);
    }
    else
        fprintf( stderr, "%d: Number or Identifier expected\n", yylineno);
}

#include <stdarg.h>
#include <signal.h>

#define MAXFIRST 16
#define SYNCH   SEMI

int legal_lookahead( first_arg)
int first_arg;
{
    /**
     * Simple error detection and recovery. Args are 0-terminated list of 
     * those tokens that can come next in the input. If the list is
     * empty, the EOF must come next. Print an error message if necessary.
     * Error recovery is performed by discarding all input symbols until 
     * one that's in the input list is found
     */
    va_list     args;
    int         tok;
    int         lookaheads[MAXFIRST], *p = lookaheads, *current;
    int         error_printed = 0;
    int         rval          = 0;
    for( ; ; )

        if( !first_arg )
        {
            if ( match( EOI ))
                rval = 1;
        }
        else
        {
            *p++ = first_arg;
            while( (tok = va_arg(args, int)) && p < &lookaheads[MAXFIRST])
                *++p = tok;
            
            while( !match( SYNCH ))
            {
                for( current = lookaheads; current < p; ++current )
                    if( match( *current ))
                    {
                        rval = 1;
                        goto exit;
                    }
                
                if(!error_printed)
                {
                    fprintf( stderr, "Line %d: Syntax error\n", yylineno);
                    error_printed = 1;
                }
                advance();
            }
        }

        exit:
            va_end( args );
            return rval; 
}