#include <stdio.h>

void main(){
 char* shell = getenv("MYSHELL"); //get the shell's mem addrs
 
 //if we have shell populated with a value we print it's pointer
 if(shell)
   printf("%x\n", (unsigned int)shell);

}
