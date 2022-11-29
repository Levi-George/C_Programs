#include <stdio.h>
//Author: Levi George
//Date: 01/13/2021
//Class/Assignment: CS232/Study 
//Purpose: Study

//INPUT: Requires no input parameters
//PROCESS: Will print out a number of fahrenheit temperatures derived from celsius temps
//OUTPUT: Returns no values
int main()
{
    //integers range from 16 bits to 32 bits in length (machine dependent)
    //floats are variables around 32 bits in length
    float fahr = 0, celsius = 0; //temp values in fahr and celsius, ints are not as accurate in this case
    int lower, upper, step; //range variables

    lower = 0;
    upper = 300;
    step = 20;

    celsius = lower;

    printf("%3s %6s\n", "Celsius", "Fahr.");
    printf("--------------------\n");

    while (celsius <= upper)
    {
        fahr = ((9.0 / 5.0) * celsius) + 32.0; //integer division truncates, hence no 5/9
        //printf("%d\t%d\n", fahr, celsius); //%d indicates a value to be substituted
        printf("%3.0f %6.1f\n", celsius, fahr);//%3d and %6d will right align the values
        celsius = celsius + step;
    }
}