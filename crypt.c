#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

main(int argc, char **argv)
{
  if(argc!=3)
    {
      printf("Usage: %s <key> <salt>\n",*argv);
      exit(EXIT_FAILURE);
    }

  printf("\"%s\"\n",crypt(*(argv+1),*(argv+2)));
}
  
