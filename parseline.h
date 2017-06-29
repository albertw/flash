#if !defined (_PARSELINE_H)
#   define _PARSELINE_H

enum parse_state  
{
  S_LWSP , S_WORD , S_QUOTED , S_ESCAPED , S_TILDE , S_DOLLAR , S_TRANSLATE
};

int parseline(char *buffer, int *argc, char **argv, int maxargc);

#endif
