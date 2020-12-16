#include "lex.h"
#include "parser.h"

typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long long int int64_t;
typedef unsigned char            uint8_t;
typedef unsigned short int       uint16_t;
typedef unsigned int             uint32_t;
typedef unsigned long long int   uint64_t;

//Type for 16-bit quantities
typedef uint16_t Elf32_Half;
typedef uint16_t Elf64_Half;

//Types for signed and unsigned 32-bit quantities
typedef uint32_t   Elf32_Word;
typedef int32_t    Elf32_Sword;
typedef uint32_t   Elf64_Word;
typedef int32_t    Elf64_Sword;


//Types for signed and unsigned 64-bit quantities
typedef uint64_t  Elf32_XWord;
typedef int64_t   Elf32_SXword;
typedef uint64_t  Elf64_Xword;
typedef int64_t   Elf64_SXword;

//Types of addresses
typedef uint32_t  Elf32_Addr;
typedef uint64_t  Elf64_Addr;

//Types for section indices, which are 16-bit
typedef uint16_t Elf32_Section;
typedef uint16_t Elf64_Section;

//Type of file offsets
typedef uint32_t Elf32_Off;
typedef uint64_t Elf64_Off;

//Type for version symbol information
typedef Elf32_Half Elf32_Versym;
typedef Elf64_Half Elf64_Versym;

//elf version info
struct sym_version {
  char *lib;
  char *version;
  int out_index;
  int prev_same_lib;
};

