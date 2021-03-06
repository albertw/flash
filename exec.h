#if !defined (_EXEC_H)
#   define _EXEC_H

#include<signal.h>
#include<termios.h>
#include <sys/types.h>

#include "parse.h"
#include "menu.h"

struct job_q
{
  enum { S_RUN, S_STOP, S_KILL, S_EXIT /*, S_FAILEDTOSTART*/ } rst;
  int signal,exit,change;
  int cstat,eflags,jflags;
  pid_t pgrp,waiton;
  time_t start;
  char *cmdline;
  struct termios *ttystate,*origstate;

  struct job_q *next,*prev;
};

#define JOBF_NONE       0x0000
#define JOBF_KEEPJOBONQ 0x0001
#define JOBF_HUPSENT    0x0002

#define EXEC_NONE       0x0000
#define EXEC_CLEARUP    0x0001
#define EXEC_NEVERPAUSE 0x0002
#define EXEC_NOPAUSE    MIF_NOPAUSE
#define EXEC_PAGE       MIF_PAGE        
#define EXEC_BACKGROUND MIF_BACKGROUND
#define EXEC_NONBGPROC  0x0004
/*      MIF_EXEC_MASK   (MIF_NOPAUSE | MIF_PAGE | MIF_BACKGROUND) */

void display_file(const char *filen);
void exec_item(struct menu_items *item);
int prep_for_exec(char *exec_line, int *argc, char **argv, int maxargc);
int do_exec (char *exec_line, int argc, char **argv, int eflags);
void handle_children(void);
void handle_snarfed_tty(void);
void init_jobq(void);
void finish_jobq(void);
void remove_job(struct job_q *j);
void clean_jobq(int volume);
int run_in_fg(struct job_q *j);
int runnable_jobs(void);
void kill_job(struct job_q *j);
void stop_job(struct job_q *j);
void run_in_bg(struct job_q *j);
char *job_status(struct job_q *j);

#define GRAB_BACK_TTY \
{\
   sigset_t omask,bmask;\
   sigemptyset(&omask);\
   sigemptyset(&bmask);\
   sigaddset (&bmask, SIGTTOU);\
   \
   sigprocmask (SIG_BLOCK, &bmask, &omask);\
   handle_snarfed_tty();\
   sigprocmask (SIG_SETMASK, &omask, NULL);\
}\

#endif

