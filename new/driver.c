#include "parser.h"

int main(void) {
  start_lex();
  do {
    get_next_token();
    switch(Token.class) {
    case IDENTIFIER: printf( "Identifier" ); break;
    case INTEGER:    printf( "Integer" ); break;
    case ERRONEOUS:  printf( "Erroneous Token" ); break;
    case EoF:        printf( "End of File psuedo token" ); break;
    default:         printf( "Operator or separator"); break;
    }
    printf( ": %s\n", Token.repr);
  } while( Token.class != EoF);
    return 0;
}
