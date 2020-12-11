/**
 * Split information about a token into two parts, the class of the token and its representation.
 * This is reflected by Token_type which has two fields
 */ 

#define EoF     256
#define DIGIT   257

typedef struct {int class; char repr; } Token_type;

extern Token_type Token;
extern void get_next_token(void);