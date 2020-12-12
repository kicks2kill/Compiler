#include "lex.h"
#include "parser.h"
#include "input.h"


/**
 * For token clsses that contain only one token which is also an ASCII character ('+' for example), the class is the ASCII value of the char itself.
 * These numbers over 255 are chosen to avoid collisions with any ASCII values of single chars.
 * 
 * The representation of a token has at least two important uses. First, it is processed in one or more phases after the parser
 * to produce semantic information; examples are a numberic value produced from an integer token and an identification in some form from an identifier token
 * 
 * The representation of a token is usally a string, implemented as a pointer. But in this compiler, all tokens are single characters.
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

#define bits_of(ch)             ( charbits[( ch )&0377])

#define is_end_of_input(ch)     (( ch ) == '\0')

#define is_uc_letter(ch)        ( bits_of(ch) & UC_LETTER_MASK)
#define is_lc_letter(ch)        ( bits_of(ch) & LC_LETTER_MASK)
#define is_letter(ch)           ( bits_of(ch) & LETTER_MASK)
#define is_operator(ch)         ( bits_of(ch) & OPERATOR_MASK)




Token_Type Token;

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

Token_type Token;

void get_next_token(void) {
  int start_dot;

  skip_layout_and_comment();
  note_token_position();

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

  Token.repr = input_to_zstring(start_dot, dot-start_dot);
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

static const char is_operator_bit[256] = {
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
