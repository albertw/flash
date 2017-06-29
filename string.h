#if !defined(__STRING_H)
#   define __STRING_H

#include <ctype.h>
#include <wchar.h>

#define CR '\n'
#define TAB '\t'

char *readline(FILE *fp);
char *findline(char *start, int *foundend);
int getline(const char *, char *);
int substr(const char *, char *, char);
int strtokenize(char *line, char delim, char **tokens, int maxtokens);
char *stradp(char *filename);
void setbasedirectory(char *b);

#define strwhite(X)	while (isspace(*(X))) (X)++

#endif
