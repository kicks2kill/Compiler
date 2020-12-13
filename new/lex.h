/**
 * Split information about a token into two parts, the class of the token and its representation.
 * This is reflected by Token_type which has two fields
 */ 

#define EoF        256
#define IDENTIFIER 257
#define INTEGER    258
#define ERRONEOUS  259

typedef struct {
  char *file_name;
  int line_number;
  int char_number;
} Position_in_File;

typedef struct {
  int class;
  char *repr;
  Position_in_File pos;
} Token_Type;

extern Token_type Token;

extern void start_lex(void);
extern void get_next_token(void);
