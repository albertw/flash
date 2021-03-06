#if !defined(_PARSE_H)
#   define _PARSE_H

#include "menu.h"

#define NO_TOKEN	-1

#define MENU_BEGIN	"Menu"
#define MENU_END	"EndMenu"
#define MODULE_BEGIN    "Module"
#define MODULE_END      "EndModule"
#define DIREC_NOBOX	"NoBox"
#define DIREC_RCFILE    "RCFile"
#define RCBLOCK_BEGIN   "RCBlock"
#define RCBLOCK_END     "EndRCBlock"
#define INCLUDE_FILE    "Include"
#define SET_MAIN_MENU   "MainMenuName"

#define MENU_BUFF	255
#define MAX_WORD	255
#define	MAX_LINE	255

int parsefile(char *);
void freemem(char *);

void setupglobalhotkeys(void);
void setuphotkeys(int numberoptions,struct menu_items **HotKeys,struct menu *menu);
void parse_error(char *message, int line);

#endif







