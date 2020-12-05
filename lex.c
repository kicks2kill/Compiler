#include "lex.h"
#include <stdio.h>
#include <ctype.h>


lex()
{
    /*
     * Lexical Anaylzers often need to know what the next input character is going to be without actually reading past it.
     * They must 'look ahead' by some number of characters.
     * Similarly, they often need to read past the end of a lexeme in order to recognize and then push back the unnecessary characters onto the input stream.  
     * Consequently, there are often extra characters that must be handled specially.    
    */
    static char input_buffer[128];
    char *current;

    current = yytext + yyleng; //init to empty string
    while(1)
    {
        while(!*current)       //return true and doesn't terminate until input_buffer holds a nonblank line
        {
            /*
                Get new lines, skipping any leading white space on the line, until a nonblank line is found
            */

           current = input_buffer;
           if(!gets(input_buffer))
           {
               *current = '\0';
               return EOI;
           }
           ++yylineno;
           while( isspace(*current))
                ++current;
        }
        for( ; *current; ++current) //Does the tokenization.
        {
            /* get next token */
            yytext = current; //point at the character
            yyleng = 1;       //hold the length

            switch(*current)
            {
                case EOF: return EOI    ;
                case ';': return SEMI   ;
                case '+': return PLUS   ;
                case '*': return TIMES  ;
                case '(': return LP     ;
                case ')': return RP     ;

                case '\n':
                case '\t':
                case ' ':break;
                default:
                    if(!isalnum(*current))
                        fprintf(stderr, "Ignoring Illegal input <%c>\n", *current);
                    else
                    {
                        while(isalnum(*current))
                            ++current;
                        yyleng = current - yytext;
                        return NUM_OR_ID;
                    }
                    break;
            }
        }
    }
}

/**
 * The parser must look at next input token without reading it.
 * We will use two subroutines: match(token) which evaluates to true if the next token in input stream matches its argument. and
 * advance() which discards the current token and advances to the next one. This strategy eliminates the necessity of a push-back subroutine.
 * 
*/

static int Lookahead = -1; //lookahead token

int match(token)
int token;
{
    //Return true if "token" matches the current lookahead symbol
    if( Lookahead == -1)
        Lookahead = lex();
    
    return token == Lookahead;
}

void advance()
{
    //advance the lookahead to next input symbol
    Lookahead = lex();
}