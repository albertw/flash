#if !defined (_SCREEN_H)
#   define _SCREEN_H

#include "parse.h"
#include <wchar.h>
#include <stdio.h>

void init_scr();
void pause_scr();
void close_scr();

void do_menu(struct menu *menu_list);
void write_str(char *s);
void clear_scr();
void clear_close_scr();
int readchar();
int get_args(char *args, int maxlen, char *title);

#endif
