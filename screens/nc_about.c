/*
   About and Help for ncurses menu system

   Copyright (C) 1996 Stephen Fegan

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation; either version 2 of the License, or (at your option) any later
   version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
   details.

   You should have received a copy of the GNU General Public License along with
   this program; if not, write to the Free Software Foundation, Inc., 675 Mass
   Ave, Cambridge, MA 02139, USA.

   please send patches (w/ explanation) or advice to: `flash@netsoc.ucd.ie'
 */

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#ifdef HAVE_NCURSES_NCURSES_H
#include <ncurses/ncurses.h>
#include <ncurses/term.h>
#else
#include <curses.h>
#include <term.h>
#endif

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

#include "ncr_scr.h"
#include "../exec.h"
#include "../menu.h"
#include "../misc.h"
#include "../parse.h"
#include "../screen.h"
#include "../set.h"

void nc_do_help(char **Body);
struct menu_instance *add_menu_instance(struct menu *menu);
struct menu_instance *rubout_menu_instance (struct menu_instance *mi);
struct menu *new_menu(void);
void display_menu(struct menu_instance *mi_c);
void up_hilite (struct menu_instance *mi);
void down_hilite (struct menu_instance *mi);
void pagedown(struct menu_instance *mi,int n);
void pageup(struct menu_instance *mi,int n);

struct anim
{
  char *frame;
  int framelen;
};

extern int color;
extern time_t Timeout;

struct objects
{
  int on;         /* On or off */
  int line,col;   /* line and col image starts at */
  int ls,cs;      /* Where (in object) to start displaying */
  int ld,cd;      /* Lines and Cols to display */
  int nlines;     /* Lines in image */
  char *d[8];
};

/*
struct objects stars[]={
  {0,0,0,0,0,-1,-1,1,"---------------------------------"},
  {0,0,0,0,0,-1,-1,3,"    /\\ ","   /  \\/\\","/`/   /  \\"},
  {0,0,0,0,0,-1,-1,3," `  _  ' ","-  (_)  -","  '   `  "},
  {0,0,0,0,0,-1,-1,3,"    ___  "," __(   ) ","(_______)"},
  {0,0,0,0,0,-1,-1,4,"      _      ","    _|_|_    ","    (o o)    ","ooO--(_)--Ooo"},
  {0,0,0,0,0,-1,-1,7,"           |  |","           |  |","           |  |","           |  |","  ________,'  `",",'            |","`.________'`__|"}
};
*/

struct anim A[]=
{{"\

    ________    ___   _____ __  __
   / ____/ /   /   | / ___// / / /
  / /_  / /   / /| | \\__ \\/ /_/ / 
 / __/ / /___/ ___ |___/ / __  /  
/_/   /_____/_/  |_/____/_/ /_/   

",1500},{"\

 
 
                          /\\  
                         /  \\/\\  
                      /`/   /  \\  
---------------------------------  
",40},{"\

 
 
                          /\\  
                         /  \\/\\  
           `  _  '    /`/   /  \\  
---------------------------------  
",40},{"\

  
  
                          /\\   
           `  _  '       /  \\/\\  
          -  (_)  -   /`/   /  \\  
---------------------------------  
",40},{"\


 
           `  _  '        /\\  
          -  (_)  -      /  \\/\\ 
            '   `     /`/   /  \\ 
--------------------------------- 
",40},{"\

 
           `  _  '  
          -  (_)  -       /\\ 
            '   `        /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",40},{"\

           `  _  ' 
          -  (_)  -  
            '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",100},{"\

           `  _  ' 
)         -  (_)  -  
_)          '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

_          `  _  ' 
 )        -  (_)  -  
__)         '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

__         `  _  ' 
  )       -  (_)  -  
___)        '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

___        `  _  ' 
   )      -  (_)  -  
____)       '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

 ___       `  _  ' 
(   )     -  (_)  -  
_____)      '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

  ___      `  _  ' 
_(   )    -  (_)  -  
______)     '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

   ___     `  _  ' 
__(   )   -  (_)  -  
_______)    '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

    ___    `  _  ' 
 __(   )  -  (_)  -  
(_______)   '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

     ___   `  _  ' 
  __(   ) -  (_)  -  
 (_______)  '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

      ___  `  _  ' 
   __(   )-  (_)  -  
  (_______) '   `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

       ___ `  _  ' 
    __(   )  (_)  -  
   (_______)'   `         /\\ 
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

        ___`  _  ' 
     __(   ) (_)  -  
    (_______)   `         /\\ 
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

         ___  _  ' 
      __(   )(_)  -  
     (_______)  `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

          ___ _  ' 
       __(   )_)  -  
      (_______) `         /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           ____  ' 
        __(   ))  - 
       (_______)`         /\\  
                         /  \\/\\ 
                      /`/   /  \\  
--------------------------------- 
",20},{"\

           `___  ' 
         __(   )  -  
        (_______)         /\\     
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           ` ___ ' 
          __(   ) -  
         (_______)        /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  ___' 
          -__(   )-  
          (_______)       /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `   ___ 
          - _((   )  
           (_______)      /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _ ___ 
          -  (_(   ) 
            (_______)     /\\   
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  ___ 
          -  (_)(   )  
            '(_______)    /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  '___ 
          -  (_)_(   )  
            ' (_______)   /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  ' ___ 
          -  (_)__(   )  
            '  (_______)  /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  '  ___ 
          -  (_) __(   )  
            '   (_______) /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  '   ___ 
          -  (_)  __(   )  
            '   `(_______)/\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  '    ___ 
          -  (_)  -__(   )
            '   ` (_______/\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  '     ___ 
          -  (_)  - __(   )  
            '   `  (______/\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  '      ___ 
          -  (_)  -  __(   )  
            '   `   (_____/\\) 
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  '       ___ 
          -  (_)  -   __(   )  
            '   `    (____/\\_) 
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  '        ___ 
          -  (_)  -    __(   )  
            '   `     (___/\\__) 
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  '         ___ 
          -  (_)  -     __(   )  
            '   `      (__/\\___) 
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

           `  _  '          ___ 
          -  (_)  -      __(   )  
            '   `       (_/\\____) 
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

                            ___ 
           `  _  '       __(   ) 
          -  (_)  -     (_/\\____) 
            '   `        /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

                             ___ 
           `  _  '        __(   ) 
          -  (_)  -      (/\\_____ 
            '   `        /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

                              ___ 
                           __( 
           `  _  '        /\\_____ 
          -  (_)  -      /  \\/\\ 
            '   `     /`/   /  \\ 
--------------------------------- 
",20},{"\

                               __ 
                            __(  
           `  _  '        /\\_____ 
          -  (_)  -      /  \\/\\ 
            '   `     /`/   /  \\ 
--------------------------------- 
",20},{"\

                                _ 
                             __( 
                          /\\(____ 
           `  _  '       /  \\/\\ 
          -  (_)  -   /`/   /  \\ 
--------------------------------- 
",20},{"\

 
                              __( 
                          /\\ (___ 
           `  _  '       /  \\/\\ 
          -  (_)  -   /`/   /  \\ 
--------------------------------- 
",20},{"\

 
                               __ 
                          /\\  (__ 
                         /  \\/\\ 
           `  _  '    /`/   /  \\ 
--------------------------------- 
",20},{"\
 

                                _ 
                          /\\   (_ 
                         /  \\/\\ 
           `  _  '    /`/   /  \\ 
--------------------------------- 
",20},{"\

 
 
                          /\\    ( 
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\

  
  
                          /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",150},{"\

  
  
                          /\\  
                         /  \\/\\ 
          _           /`/   /  \\ 
--------------------------------- 
",20},{"\
  

  
                          /\\  
          _              /  \\/\\ 
        _|_|_         /`/   /  \\ 
--------------------------------- 
",20},{"\
  

  
          _               /\\  
        _|_|_            /  \\/\\ 
        (o o)         /`/   /  \\ 
--------------------------------- 
",10},{"\
  

  
          _               /\\  
        _|_|_            /  \\/\\ 
        (o o)         /`/   /  \\ 
----ooO--(_)--Ooo---------------- 
",300},{"\
   `.________'`__|   

 
          _               /\\  
        _|_|_            /  \\/\\ 
        (o o)         /`/   /  \\ 
----ooO--(_)--Ooo---------------- 
",20},{"\
   ,'            |
   `.________'`__|   

          _               /\\  
        _|_|_            /  \\/\\ 
        (o o)         /`/   /  \\ 
----ooO--(_)--Ooo---------------- 
",20},{"\
     ________,'  `
   ,'            |
   `.________'`__|   
          _               /\\  
        _|_|_            /  \\/\\ 
        (o o)         /`/   /  \\ 
----ooO--(_)--Ooo---------------- 
",20},{"\
              |  |
     ________,'  `
   ,'            |
   `.________'`__|        /\\  
        _|_|_            /  \\/\\ 
        (o o)         /`/   /  \\ 
----ooO--(_)--Ooo---------------- 
",20},{"\
              |  |
              |  |   
     ________,'  `
   ,'            |        /\\  
   `.________'`__|       /  \\/\\ 
        (o o)         /`/   /  \\ 
----ooO--(_)--Ooo---------------- 
",20},{"\
              |  |
              |  |
              |  |
     ________,'  `        /\\  
   ,'            |       /  \\/\\ 
   `.________'`__|    /`/   /  \\ 
----ooO--(_)--Ooo---------------- 
",20},{"\
              |  |
              |  |
              |  |
              |  |        /\\  
     ________,'  `       /  \\/\\ 
   ,'            |    /`/   /  \\ 
---`.________'`__|--------------- 
",50},{"\
              |  |
              |  |
              |  |
     ________,'  `        /\\  
   ,'            |       /  \\/\\ 
   `.________'`__|    /`/   /  \\ 
--------------------------------- 
",20},{"\
              |  |
              |  |   
     ________,'  `
   ,'            |        /\\  
   `.________'`__|       /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
              |  |
     ________,'  `
   ,'            |
   `.________'`__|        /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
     ________,'  `
   ,'            |
   `.________'`__|   
                          /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
     ,'            |
     `.________'`__|   

                          /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
       `.________'`__|   


                          /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
         `.________'`__|   


                          /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
           `.________'`__|   


                          /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
             `.________'`__|   


                          /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
               `.________'`__|   


                          /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
                 ,'            |
                 `.________'`__|   

                          /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
                    ________,'  `
                  ,'            |
                  `.________'`__|   
                          /\\  
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
                             |  |
                    ________,'  `
                  ,'            |
                  `.________'`__|   
                         /  \\/\\ 
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
                             |  |
                             |  |
                    ________,'  `
                  ,'            |
                  `.________'`__|   
                      /`/   /  \\ 
--------------------------------- 
",20},{"\
                             |  |
                             |  |
                             |  |
                    ________,'  `
                  ,'            |
                  `.________'`__|   
--------------------------------- 
",20},{"\
                             |  |
                             |  |
                             |  |
                             |  |
                    ________,'  `
                  ,'            |
------------------`.________'`__|   
",100},{"\
                             |  |
                             |  |
                             |  | 
                             |  |
                    ________,'  `
     `  _  '      ,'            |
------------------`.________'`__|
",40},{"\
                             |  |
                             |  |
                             |  | 
                             |  |
     `  _  '        ________,'  `
    -  (_)  -     ,'            |
------------------`.________'`__|
",40},{"\
                             |  |
                             |  |
                             |  | 
     `  _  '                 |  |
    -  (_)  -       ________,'  `
      '   `       ,'            |
------------------`.________'`__|
",40},{"\
                             |  |
                             |  |
     `  _  '                 |  | 
    -  (_)  -                |  |
      '   `         ________,'  `
                  ,'            |
------------------`.________'`__|
",20},{"\
                            |  |
                            |  |
     `  _  '                |  |
    -  (_)  -      ________,'  `
      '   `      ,'            |
                 `.________'`__|
---------------------------------
",20},{"\
                          |  |
     `  _  '              |  |
    -  (_)  -    ________,'  `
      '   `    ,'            |
               `.________'`__|
               
---------------------------------
",20},{"\
                       |  |
     `  _  '  ________,'  `
    -  (_)  ,'            |
      '   ` `.________'`__|
              
               
---------------------------------
",20},{"\
                     |  |
     `  _  '________,'  `
    -  (_),'            |
      '   `.________'`__|
              
               
---------------------------------
",20},{"\
   `  _  '          |  |
  -  (_)  -________,'  `
    '   `,'            |
         `.________'`__|
              
               
---------------------------------
",20},{"\
-  (_)  -          |  |
  '   `   ________,'  `
        ,'            |
        `.________'`__|
              
               
---------------------------------
",20},{"\
'   `     ________,'  `
        ,'            |
        `.________'`__|
       
       
               
---------------------------------
",20},{"\
        ,'            |
        `.________'`__|

       
       
               
---------------------------------
",20},{"\
        `.________'`__|


       
       
               
---------------------------------
",20},{"\






---------------------------------
",150},{"\





---------------------------------
",20},{"\




---------------------------------

     Written and Directed by
",20},{"\



---------------------------------

     Written and Directed by
",20},{"\


---------------------------------

     Written and Directed by

          Stephen Fegan
",20},{"\

---------------------------------

     Written and Directed by

          Stephen Fegan
",20},{"\
---------------------------------

     Written and Directed by

          Stephen Fegan
",20},{"\

     Written and Directed by

          Stephen Fegan


              With
",20},{"\
     Written and Directed by

          Stephen Fegan


              With
",20},{"\

          Stephen Fegan


              With

       The EFTIWALL fonts
",20},{"\
          Stephen Fegan


              With

       The EFTIWALL fonts
",20},{"\


              With

       The EFTIWALL fonts

               by
",20},{"\

              With

       The EFTIWALL fonts

               by
",20},{"\
              With

       The EFTIWALL fonts

               by

    Michel Eftimakis (c) 1995
",20},{"\

       The EFTIWALL fonts

               by

    Michel Eftimakis (c) 1995
",20},{"\
       The EFTIWALL fonts

               by

    Michel Eftimakis (c) 1995
",20},{"\

               by

    Michel Eftimakis (c) 1995
",20},{"\
               by

    Michel Eftimakis (c) 1995
",20},{"\

    Michel Eftimakis (c) 1995
",20},{"\
    Michel Eftimakis (c) 1995
",20},{"\

",200},{"\


Oh and eh..
",4}
};


void nc_about(void)
{
  WINDOW *b_win,*f_win,*a_win;
  int nframes=sizeof(A)/sizeof(struct anim);
  int i,finished=0;
  chtype chf,cha;

  char t1[]="flash version " VERSION;
  char t2[]="Copyright 1997";
  char t3[]="Stephen Fegan";

  b_win=newwin(18,60,NC_VCENTRE_SC(18),NC_HCENTRE_SC(60));
  werase(b_win);
  box(b_win,0,0);
  wnoutrefresh(b_win);

  f_win=derwin(b_win,16,58,1,1);
  chf= ' ' | A_BOLD;
  wattrset (f_win, A_BOLD);
  if(color==TRUE)
    {
      chf|=COLOR_PAIR((MENU_EXEC+1));
      wattron(f_win, COLOR_PAIR((MENU_EXEC+1)));
    }

  werase(f_win);
  wbkgd(f_win,chf);

  a_win=derwin(f_win,8,36,8,NC_CENTRE_AB(58,34));
  wattrset (a_win, A_BOLD);
  cha= ' ' | A_BOLD;
  if(color==TRUE)
    {
      wattron(a_win, COLOR_PAIR((MENU_TITLE+1)));
      cha|=COLOR_PAIR((MENU_TITLE+1));
    }

  while(!finished)
    {
      for(i=0;(i<nframes)&&(!finished);i++)
        {
	  display_screen(0);
	  
	  touchwin(b_win);  wnoutrefresh(b_win);

          werase(a_win);
          wbkgd(a_win,cha);
          mvwaddstr(a_win,0,0,A[i].frame);
          wnoutrefresh(a_win);
          
          mvwaddstr(f_win,2,NC_CENTRE_AB(58,strlen(t1)),t1);
          mvwaddstr(f_win,4,NC_CENTRE_AB(58,strlen(t2)),t2);
          mvwaddstr(f_win,6,NC_CENTRE_AB(58,strlen(t3)),t3);
          wnoutrefresh(f_win);
          
          doupdate();
          
          timeout(A[i].framelen*10);

	  do
	    {
	      errno=0;
	      if(getch()!=ERR)finished=1;
	      if(errno==EIO)GRAB_BACK_TTY
	      if(checktty()==0)finished=1;
	    }while(errno!=0&&errno!=ENOTTY);
        }
      finished=1;
    }

  delwin(a_win);
  delwin(f_win);
  delwin(b_win);
}


void nc_menu_help(void)
{
  char *Help[]=
    {
      "                        < Menu Windows >                         ",
      " ",
      " This section describes the control keys available on the menus. ",
      " Note that the menus may have hotkeys and global hotkeys defined ",
      " on them that will augment those listed below. ",
      " ",
      " Key            Action ",
      " ---            ------ ",
      " Up,    Ctrl-P  Move cursor bar to previous option of menu. ",
      " ",
      " Down,  Ctrl-N  Move cursor bar to next option of menu. ",
      " ",
      " Left,  Ctrl-B  Exit current menu and return to last. ",
      " ",
      " Right, Enter   Select highlighted item in a menu. ",
      " ",
      " Ctrl-E         Page down menu if menu is longer than one page. ",
      " ",
      " Ctrl-W         Page up menu. ",
      " ",
      " Ctrl-L         Redraw the screen. This is useful if someone ",
      "                sends you a message that writes on the menus.",
      " ",
      " Ctrl-D         Logout from flash. ",
      " ",
      " TAB            Enter the task switch menu. This allows you to ",
      "                control background and stopped processes. You ",
      "                can Stop, Kill, Background or Foreground tasks.",
      " ",
      " Ctrl-K         Show Global hotkey menu. This menu can be used ",
      "                to define keys that will select frequently used ",
      "                items easily. Hotkeys that are defined in this ",
      "                menu are available on all menus. ",
      " ",
      " Ctrl-G         View the GNU Public License (GPL). ",
      " ",
      " Ctrl-H         Some more help. ",
      " ",
      " L              Lock the screen. You are prompted for a password ",
      "                and flash only resumes when the password is ",
      "                reentered correctly.",
      " ",
      " A              About flash. Essential viewing.... ",
      " ",
      " ",
      "                    < Task Selection Menu >                      ",
      " ",
      " This section describes the keys available on the task selection ",
      " window. This window allows you to manage / switch tasks. ",
      " ",
      " Key            Action ",
      " ---            ------ ",
      " TAB            Advance to next active task. Active tasks are ",
      "                process groups that are marked as running or ",
      "                as stopped. ",
      " ",
      " Enter          Run / Resume an active task in the foreground. ",
      " ",
      " B              Run an active task in the background. ",
      " ",
      " S              Stop a task that is running in the background. ",
      " ",
      " K              Send a task a signal. The first time this option ",
      "                is used a 'Hangup' is sent, the second time a ",
      "                'Kill' signal is sent. ",
      " ",
      " ",
      "                   < Argument Entry Window >                     ",
      " ",
      " The argument entry window allows additional arguments to be ",
      " supplied to programs being run. ",
      " ",
      " Key            Action ",
      " ---            ------ ",
      " LEFT           Move cursor left, scrolling if necessary. ",
      " ",
      " RIGHT          Move cursor right, scrolling if necessary. ",
      " ",
      " Backspace      Delete character to left of cursor. ",
      " ",
      " Del / Ctrl-D   Delete character under cursor. ",
      " ",
      " Ctrl-A         Go to begining of line. ",
      " ",
      " Ctrl-E         Go to end of enetered text. ",
      " ",
      " Ctrl-U         Delete all entered text. ",
      " ",
      " Ctrl-L         Redraw screen. ",
      NULL
    };

  nc_do_help(Help);
}

void nc_do_help(char **Body)
{
  struct menu_items Head[]={
    {MENU_TITLE,"flash version " VERSION,NULL,NULL,MIF_CENTRE|MIF_HEAD,'k',Head+1,NULL},
    {MENU_TITLE,  "  ",NULL,NULL,MIF_HEAD,'k',Head+2,Head},
    {MENU_TITLE,"Copyright 1997  Stephen Fegan",NULL,NULL,MIF_CENTRE|MIF_HEAD,'k',NULL,Head+1}};
  
  struct menu_items Tail[]={
    {MENU_EXEC,"(Up/Down to scroll, Space for next page, 'q' to quit help)",NULL,NULL,MIF_CENTRE|MIF_TAIL,'k',NULL,NULL}};

  struct menu_items *data,*Data=NULL,BodyTemplate[]={
    {MENU_NOP,NULL,NULL,NULL,MIF_NONE,'k',NULL,NULL}};

  struct menu *help;

  struct menu_instance *mi_c;

  int n=0,gotch;

  while(*Body!=NULL)
    {
      data = (struct menu_items *) xmalloc (sizeof (struct menu_items));
      memcpy(data,BodyTemplate,sizeof(*data));
      data->name=*Body;
      
      data->prev=Data;
      if(Data!=NULL)Data->next=data;
      Data=data;
      Body++;
      n++;
    }

  while(Data->prev!=NULL)Data=Data->prev;

  help=new_menu();

  help->head=Head;
  help->tail=Tail;
  help->data=Data;

  help->flags |= MF_SCROLLBAR;
  if((n+8)>(LINES-2))help->flags |= MF_FIXEDSIZE, help->nlines = LINES-2;

  mi_c = add_menu_instance(help);

  render_instance (mi_c);
  display_screen (1);

  timeout(Timeout);

  do
    {
      do
	{
	  errno=0;
	  gotch=getch();
	  if(errno==EIO)GRAB_BACK_TTY
	}while(errno!=0&&errno!=ENOTTY);
  
      switch (gotch)
	{
	case ERR:
	  if(checktty()==0)return;
	  display_screen(1);
	  break;

	case KEY_DOWN:
	case 14: /* Ctrl N */
	  pagedown(mi_c,1);
	  display_screen(1);
	  break;

	case KEY_UP:
	case 16: /* Ctrl P */
	  pageup(mi_c,1);
	  display_screen(1);
	  break;

	case ' ':
	  pagedown(mi_c,0);
	  display_screen(1);
	  break;

	case 'b':
	case 'B':
	  pageup(mi_c,0);
	  display_screen(1);
	  break;

	case 12:
	  display_screen(1);
	  wrefresh (curscr);
	  break;
	}
    }while((gotch!='q')&&(gotch!='Q')&&(gotch!=KEY_LEFT)
	   &&(gotch!='\n')&&(gotch!='\r')&&(gotch!='\004'));

  timeout(-1);

  while(Data->prev!=NULL)Data=Data->prev;
  while(Data!=NULL)
    {
      data=Data->next;
      free(Data);
      Data=data;
    }
  free(help);

  rubout_menu_instance (mi_c);
}




