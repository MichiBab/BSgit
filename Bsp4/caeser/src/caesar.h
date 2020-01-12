#ifndef cae_h
#define cae_h


static void encode(char *input, char *output, int outputSize, int shiftNum);
static void decode(char *input, char *output, int outputSize, int shiftNum);
static int is_ascii(char c);
static int shift_char(char* c, int shiftNum);
static int unshift_char(char* c, int shiftNum);

#endif