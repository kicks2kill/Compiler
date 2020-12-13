#include "parser.h"
#include "lex.h"

int main(void) {
  start_lex();
  get_next_token();
  require(input());
  return 0;
}

void error(void){
  printf("Error in expression\n"); exit(1);
}
