#define EOI       0
#define SEMI      1
#define PLUS      2
#define TIMES     3
#define LP        4
#define RP        5
#define NUM_OR_ID 6

char *yytext   = ""; /* current Lexeme (not '\0' terminated) */
int   yyleng   = 0;/* Lexeme length*/
int   yylineno = 0; /*Input line number*/