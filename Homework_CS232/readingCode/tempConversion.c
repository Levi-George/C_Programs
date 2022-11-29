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
    //integers range from 16 bits to 32 bits in length (machine dependent)
    //floats are variables around 32 bits in length
    float fahr = 0, celsius = 0; //temp values in fahr and celsius, ints are not as accurate in this case
    int lower, upper, step; //range variables

    lower = 0;
    upper = 300;
    step = 20;

    fahr = lower;

    printf("%3s %6s\n", "Fahr.", "Celsius");
    printf("--------------------\n");

    while (fahr <= upper)
    {
        celsius = (5.0 / 9.0) * (fahr-32.0); //integer division truncates, hence no 5/9
        //printf("%d\t%d\n", fahr, celsius); //%d indicates a value to be substituted
        printf("%3.0f %6.1f\n", fahr, celsius);//%3d and %6d will right align the values
        fahr = fahr + step;
    }
}

//errors - 1
//numbers were listed as rediculously large or zero
//the cause was the continued use of %3d and %6d for our output after switching to float
//for fahr and celsius
//This is because %d indicates printing an integer
//%f indicates a floating point value
//the first number indicates the width of output 
//the .# indicates how many numbers after the decimal

//%o for octal
//%x for hex
//%c for char
//%s for strings
//%% for %