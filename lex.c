#include  <stdio.h>
#include "lex.h"

static char *input;
static int dot;        //dot position in input
static int input_char; //char at dot position

#define next_char()

Token_Type Token;

void start_lex(void) {
    input = get_input();
    dot = 0; 
    input_char = input[dot];
}

void get_next_token(void) {
    int start_dot;
    skip_layout_and_comment();
    /*Now we are at the start of a token or at EOF*/
    note_token_position();

    /*Split on first char of token*/
    start_dot = dot;
    if(is_end_of_input(input_char)){
        Token._class = EoF;
        Token.repr = "<EoF>";
        return;
    }
    if( is_letter(input_char)) {recognize_indentifier();}
    if( is_digit(input_char)) {recognize_integer();}
    else
    if( is_operator(input_char) || is_separator(input_char)){
        Token._class = input_char;
        next_char();
    }
    else { Token._class = ERRONEOUS; next_char();}

    Token.repr = input_to_zstring(start_dot, dot - start_dot);
}

void skip_layout_and_comment(void) {
    while(is_layout(input_char)) {next_char();}
    while(is_comment_starter(input_char)) {
        next_char();
        while(!is_comment_stopper(input_char)){
            if(is_end_of_input(input_char)) return;
            next_char();
        }
        next_char();
        while(is_layout(input_char)) {next_char();}
    }
}

void recognize_identifier(void) {
    Token._class = IDENTIFIER; next_char();
    while(is_letter_or_digit(input_char)) {next_char();}

    while(is_underscore(input_char) && is_letter_or_digit(input[ dot + 1 ])) {
        next_char();
        while(is_letter_or_digit(input_char)) {next_char();}
    }
}

void recognize_integer(void) {
    Token._class = INTEGER; next_char();
    while(is_digit(input_char)) {next_char();}
}