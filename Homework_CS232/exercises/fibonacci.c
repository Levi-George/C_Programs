#include <stdio.h>
//Author: Levi George
//Date: 1/14/2021
//Class/Assignment: CS232/Practice
//Purpose: Practice to rebuild familiarity with C and Programming


int main()
{
    unsigned int one = 0, two = 1;

    for(int i = 0; i < 32; i++)
    {
       
        printf("%10d\n", (one));
        one += two;
        
    }
}