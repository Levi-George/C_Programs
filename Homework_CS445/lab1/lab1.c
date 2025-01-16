//Author: Levi George
//Date: 01/11/23
//Class: CS445
//Purpose: write env variables to cmdline

#include <stdio.h>
#include <stdlib.h>

extern char** environ;
int main(int argc, char* argv[])
{
   int i = 0;
   while (environ[i] != NULL)
   {
      printf("%s\n", environ[i++]);


   }
   printf("SHELL : %s\n", getenv("SHELL"));

   char *strPTR = getenv("SHELL");//get address of shell into new var

   printf("SHELL ADDR : %p\n", strPTR);

}
