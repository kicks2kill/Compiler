
#include "lex.h"
#include "parser.h"
#include "input.h"

/**
 * Let's imagine that we stop the attempt to match the input to a given token description before it has either succeded or failed.
 * We would effectively have four components; the part of the input that has already been matched, the part of the regular expression that has matched it
 * the part of the regular expression that must still find a match, and the rest of the input that will provide the match.
 * 
 * The two parts of the regular expression are recombined into the original token description, with the gap marked by a dot and is called a dotted item.
 */

static char *input;
static int dot;        /* dot position in input  */
static int input_char; /* character at dot position  */

#define next_char()
#define is_end_of_input(ch)     ((ch) == '\0')
#define is_layout(ch)           (!is_end_of_input(ch) && (ch) <= ' ')
#define is_comment_starter(ch)  ((ch) == "#")
#define is_comment_stopper(ch)  ((ch) == "#" || (ch) == '\n')

#define is_digit(ch)            ('0' <= (ch) && (ch) <= '9')
#define is_letter_or_digit(ch)  (is_letter(ch) || is_digit(ch))
#define is_underscore(ch)       ((ch) == '_')


#define is_separator(ch)        (strchr(" ;,(){} ", (ch)) != 0)

#define UC_LETTER_MASK(1 << 1)
#define LC_LETTER_MASK( 1 << 2)
#define OPERATOR_MASK(1 << 5)
#define LETTER_MASK             ( UC_LETTER_MASK | LC_LETTER_MASK )

#define bits_of(ch)             ( charbits[( ch )&0377]) /* ANDing by octal number 0377 is full in binary. */

#define is_end_of_input(ch)     (( ch ) == '\0')

#define is_uc_letter(ch)        ( bits_of(ch) & UC_LETTER_MASK)
#define is_lc_letter(ch)        ( bits_of(ch) & LC_LETTER_MASK)
#define is_letter(ch)           ( bits_of(ch) & LETTER_MASK)
#define is_operator(ch)         ( bits_of(ch) & OPERATOR_MASK)

void start_lex(void) {
  input = get_input();
  dot = 0;
  input_char = input[dot];

}


static int is_layout_char(int ch) {
    switch(ch) {
        case ' ': 
        case '\t':
        case '\n':
            return 1;
        default:
            return 0;
    }
}

Token_Type Token;

void get_next_token(void) {
  int start_dot;

  skip_layout_and_comment();

  start_dot = dot;
  if( is_end_of_input( input_char ) {
      Token.class = EoF;
      Token.repr = "<EoF>";
      return;
   }
    if( is_letter( input_char )) { recognize_identifier();}
    else
      if( is_digit( input_char )) {recognize_integer();}
      else
	if( is_operator( input_char )) || is_separator( input_char )){
    Token.class = input_char;
    next_char();
  }
  else {Token.class = ERRONEOUS; next_char();}

  //TODO
  // Token.repr = input_to_zstring(start_dot, dot-start_dot);
}

void skip_layout_and_comment(void) {
  while( is_layout( input_char )) { next_char();}
  while( is_comment_starter( input_char )) {
    next_char();
    while(!is_comment_stopper(input_char)) {
      if(is_end_of_input( input_char )) return;
      next_char();
    }
    next_char();
    while( is_layout( input_char )) { next_char();}
  }
}

void recognize_identifier(void){
  Token.class = IDENTIFIER;
  next_char();
  while( is_letter_or_digit( input_char )) { next_char(); }

  while( is_underscore( input_char ) && is_letter_or_digit( input[ dot + 1 ])) {
    next_char();
    while( is_letter_or_digit( input_char )) { next_char(); }
  }
 }

void recognize_integer(void) {
  Token.class = INTEGER;
  next_char();
  while( is_digit( input_char )) {next_char();}
}

static int Parse_operator(Operator *oper) {
    if(Token.class == '+') {
        *oper = '+';
        get_next_token();
        return 1;
    }
    if(Token.class == '*') {
        *oper = '*';
        get_next_token();
        return 1;
    }
    return 0;
}
/* We need an 8-bit character which can have at most 256 values, and the outcome of the macro is one bit.
   This suggests representing the table of answers as an array A of 256 1-bit elements, in which element A[ch] contains the result for parameter ch.
   Since few languages offer 1-bit arrays, and if they do - it may be slow. We decide to sacrifice 7 x 256 bits and allocate an array of 256 bytes for the answer.
   ---
   Perform a table lookup and manually allocate instead of relying on C compiler.
   Here, we can notice that the leftmost 7 bits of each byte are always zero, and the idea suggests iteself to use these bits to store outcomes of some of the other functions.
   So we can use the proper bit for a function that will be extracted by ANDing with a mask in which one bit is set to 1 at the proper bit position.
 */
static const char charbits[256] = {
  0000, /* Position 0 */
  0040, /* '*', Position 42 */
  0040, /*  '+'  */
  0000, /* Position 64 */
  0002, /* 'A' */
  0002, /* 'B'  */
  0000, /* Position 96 */
  0004, /* 'a' */
  0004, /* 'b' */ 
  0000 /* Position 255 */
};

  
static int Parse_expression(Expression **expr_p) {
    Expression *expr = *expr_p = new_expression();

    if(Token.class == DIGIT) {
        expr->type = 'D';
        expr->value = Token.repr - '0';
        get_next_token();
        return 1;
    }

    if(Token.class == '(') {
        expr->type = 'P';
        get_next_token();
        if( !Parse_expression(&expr->left)) {
            Error("Missing expression");
        }

        if(!Parse_operator(&expr->oper)) {
            Error("Missing opertor");
        }

        if(!Parse_expression(&expr->right)) {
            Error("Missing expression");
        }

        if(Token.class != ')') {
            Error("Missing ')' ");
        }
        get_next_token();
        return 1;
    }

    free_expression(expr);
    return 0;
}
