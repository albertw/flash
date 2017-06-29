/*
   Manipulates strings for easier parsing

   Copyright (C) 1996  Stephen Fegan

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

   please send patches or advice to: `flash@netsoc.ucd.ie'
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "string.h"
#include "misc.h"

char *basedirectory=PREFIX;
char *libdirectory=LIBPREFIX;
char escape = ESCAPE;

char *readline(FILE *fp)
{
  static char *linebuffer=NULL;
  static int linebufferlen=0;
  int linebufferused=0;
  int nread;
  
  while(1)
    {
      /* Allocate space if there is none left */      
      if((linebufferlen-linebufferused)<=1)
	{
	  linebufferlen=(linebufferlen>0)?2*linebufferlen:256;
	  linebuffer=xrealloc(linebuffer,linebufferlen);
	}
      
      if((fgets(linebuffer,linebufferlen-linebufferused,fp)==NULL)
	 &&(linebufferused==0))return NULL;

      nread=strlen(linebuffer+linebufferused);
      linebufferused+=nread;

      if((nread==0)||(*(linebuffer+linebufferused-1)=='\n'))
	{
	  char *linestart;
	  while((linebufferused)&&(isspace(*(linebuffer+linebufferused-1))))linebufferused--;
	  for(linestart=linebuffer;(linestart<(linebuffer+linebufferused))&&(isspace(*linestart));linestart++);
	  *(linebuffer+linebufferused)='\0';
	  return linestart;
	}
    }
}

/* 
   Find a line and terminate it with a '\0'. Foundend is set when
   the line we find is already terminated with a '\0' rather than a
   '\n' denoting the end of the input string
*/

char *findline(char *start, int *foundend)
{
  char *lineseek;

  lineseek = strchr (start, '\n');
  
  if(lineseek==NULL)
    {
      lineseek = strchr (start, '\0');
      *foundend = 1;
    }
  
  *lineseek='\0';

  return(lineseek);
}

/*
   Break a string into a number of tokens (at most maxtokens)
   using delim as a delimiter. Return the number of tokens found.
   Note that eg A:B:C: counts as 4 tokens "A","B","C","" ie the
   end of line is treated as a delimiter too.
*/
  
int strtokenize(char *line, char delim, char **tokens, int maxtokens)
{
  char **thistoken=tokens;
  char *copyto=line;
  int tokensfound=0,end=0;
  

  while((!end)&&(tokensfound<maxtokens))
    {
      copyto=line;
      *thistoken=line;
      
      while((*line!=delim)&&(*line))
	{
	  if(*line==escape)
	    {
	      if(*(line+1))
		++line;
	      else
		*line='\0';
	    }

	  if(copyto!=line)*copyto=*line;
	  if(*line)++line,++copyto;
	}

      if(!*line)end=1;
      else *copyto='\0',line++;
      thistoken++;
      tokensfound++;
    }
  return tokensfound;
}

/*
   stradp adds the default prefix (LIBPREFIX) to a filename if the
   filename does not contain a "/". It returns a pointer to the
   static buffer prefixed_filename.
*/

char prefixed_filename[256];

char *stradp(char *filename)
{
  /* Is there a / in the file - if so don't add the default prefix */
  if((*filename=='.')||(*filename=='/'))return filename;

  strcpy(prefixed_filename,basedirectory);
  strcat(prefixed_filename,libdirectory);
  if(strlen(prefixed_filename)&&
     prefixed_filename[strlen(prefixed_filename)-1]!='/')
    strcat(prefixed_filename,"/");
  strcat(prefixed_filename,filename);
  return prefixed_filename;
}

void setbasedirectory(char *b)
{
  basedirectory=b;
}

