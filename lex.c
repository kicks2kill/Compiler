#include "lex.h"
#include <stdio.h>
#include <ctype.h>

char *yytext   = ""; /* current Lexeme (not '\0' terminated) */
int   yyleng   = 0;/* Lexeme length*/
int   yylineno = 0; /*Input line number*/

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
        while(!*current)       //return true
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
        for( ; *current; ++current)
        {
            /* get next token */
            yytext = current;
            yyleng = 1;

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
    //advance the lokoahead to next input symbol
    Lookahead = lex();
}