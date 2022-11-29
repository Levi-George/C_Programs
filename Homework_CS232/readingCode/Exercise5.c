#include <stdio.h>
//Author: Levi George
//Date: 01/13/2021
//Class/Assignment: CS232/Study 
//Purpose: Study

//INPUT: Requires no input parameters
//PROCESS: Will print out a number of celsius temperatures derived from fahrenheit temps
//OUTPUT: Returns no values
int main()
{
    int fahr;

    for (fahr = 300; fahr >= 0; fahr -= 20)
    {
        printf("%3d %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }
}