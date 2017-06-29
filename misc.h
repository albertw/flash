#if !defined (_MISC_H)
#   define _MISC_H

#include<termios.h>

void error(char *str);
void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
int mail_check(int check_type);
int tty_cbreak(int fd, struct termios *saved);
int tty_reset(int fd, struct termios *saved);
char *shacrypt(char *key,char *salt);

#endif
#define COMMENT '#' 
#define VERSION "0.9.4"
