#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

#include "tree.h"
#include "set.h"
#include "menu.h"
#include "config.h"
#include "misc.h"

FILE *debugfp;
extern char *progname;
extern struct dict *SetVariables;
extern struct dict *Menus;
extern char *basedirectory;

extern char **environ;

static void showbasedir(void);
static void showmenus(void);
static void showsetvariables(void);
static void showenviron(void);

void pressanykey(void);

void set_debug_file(char *fn)
{
  debugfp=fopen(fn,"w");
  if(debugfp==NULL)
    {
      fprintf(stderr,"%s: Could not open %s\n",progname,fn);
      perror(progname);
      debugfp=stderr;
    }
  fprintf(debugfp,"flash version "VERSION", Copyright (C) 1996 Stephen Fegan <flash@netsoc.ucd.ie>\n\n");
}

void close_debug_file(void)
{
  if(debugfp!=stderr)fclose(debugfp);
}

void showconfig(char *fn)
{
  fprintf(debugfp,"Menu file: %s\n",fn);
  showbasedir();
  showmenus();
  showenviron();
  showsetvariables();
  if(debugfp==stderr)pressanykey();
  return;
}

static void showmenu(struct menu *m)
{
  if(m->name)fprintf(debugfp,"  %s\n",m->name);
}

static void showmenus(void)
{
  fprintf(debugfp,"\nMenus\n");

  if(Menus!=NULL)
    {
      relink_inorder(Menus);
      visit_nodes(Menus,(void (*)(void *))showmenu);
    }
}

static void showsetvariable(struct set_node *n)
{
  if(n->variable)fprintf(debugfp,"  %s=%s\n",n->variable,n->value);
}

static void showsetvariables(void)
{
  fprintf(debugfp,"\nVariables\n");

  if(SetVariables!=NULL)
    {
      relink_inorder(SetVariables);
      visit_nodes(SetVariables,(void (*)(void *))showsetvariable);
    }
}

static void showenviron(void)
{
  char **envp;
  envp=environ;

  fprintf(debugfp,"\nEnvironment\n");
  while(*envp!=NULL)fprintf(debugfp,"  %s\n",*envp++);
}

static void showbasedir(void)
{
  fprintf(debugfp,"\nBase dir:  %s\n",basedirectory);
}
