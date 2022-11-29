#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    bufSize = 12
} size;

void explode(const int code)
{
    printf("The bomb has exploded: %d", code);
    //exit(0);
}

void readInput(char buffer[], const int bufferSize)
{
    int i;
    int c;

    for(i = 0; i <= bufferSize; ++i)
    {
        c = getchar();

        if(c == '\r')
        {
            c = getchar();
        }

        if(c == '\n')
        {
            break;
        }
        else if(i<bufferSize)
        {
            buffer[i] = (char)c;
        }
    }
}

void part1()
{
    enum bufferSize { bufferSize = 12 };
    char buffer[bufferSize];
    char target[] = "Careful";

    int i;
    const int n = strlen(target);

    readInput(buffer, bufferSize);

    for(i = 0; i < n; i++)
    {
        if(buffer[i] != target[i])
        {
            explode(1);
        }
    }
}

int part2()//93 - 17 = 76
{
    int z;

    enum bufferSize { bufferSize = 12 };
    char buffer[bufferSize];

    readInput(buffer, bufferSize);

    z = atoi(buffer);

    if(!z)
        explode(20);

    z += 17;

    if(z != 93)
        explode(21);

    return z / 2;
}

void part3()// Zjj
{
    int i;

    enum bufferSize { bufferSize = 12 };
    char buffer[bufferSize];

    readInput(buffer, bufferSize);

    i = 0;
    while(i < bufferSize && (buffer[i] == 'Z' || buffer[i] == 'j'))
    {
        i++;
    }

    if(i != 3)
        explode(70);
}

void part4(const int x)//x = 93 / 2 = 46
{
    int i, y, z;
    enum bufferSize { bufferSize = 12 };
    char buffer[bufferSize];

    readInput(buffer, bufferSize);

    z = atoi(buffer);
    if(!z)
        explode(20);

    y = z;
    for(i = 0; i < 50; i++)//this is a trick
    {
        y += z * i * y;
    }

    if(z + 9 != x)
    {
        explode(21);
    }
}

void part5(char buffer[], const int bufferSize)
{
    const char allowed[] = "az3hoPeACce6";

    int i, j, x;

    for(i = 0; i < bufferSize; ++i)
    {
        char c;
        
        x = getchar();

        if(x == EOF)
        {
            explode(10);
        }
        else if(x == '\n')
        {
            explode(11);
        }

        c = (char)x;
        for(j = 2; j < 10; ++j)//we start at h, we cover hoPeACce6
        {
            if(c == allowed[j])
            {
                buffer[i] = c + 1; //
                break; 
            }
        }

        if(j == 10)
        {
            explode(12);
        }
    }

    j = 0;
    for(i = 1; i < bufferSize; ++i)
    {
        if(buffer[i] != buffer[i - 1])
        {
            j = 1;
            break;
        }
    }

    if(j == 0)
        explode(13);

    x = getchar();//this will attempt to mess with you
    if(x == '\r')
        x = getchar();
    if(x != '\n')
        explode(14);
}

void part6(const char buf1[])//buf1 = ipQfBDdf7, buf2 should be 7fdDBfQpi
{
    enum bufferSize { bufSize = 10 };

    char buf2[bufSize];
    int i;
    readInput(buf2, bufSize);

    for(i = 0; i < bufSize; ++i)
    {
        if(buf1[i] != buf2[bufSize - 1 - i])
        {
            explode(-44);
        }
    }
}

void part7(int *x) //receives location of 46
{
    int y = 29;
    int *z = &y; //location of y
    int k;

    enum bufSize { bufSize = 12 };
    char buffer[bufSize];

    *z -= 3; // = 26

    readInput (buffer, bufSize);

    k = atoi(buffer);

    if(k + *x != y) //y is 29, x is 46, k should be -17
    {
        explode(70);
    }
}

int main()
{
    enum bufferSize { bufSize = 10 };
    char buffer[bufSize];
    int result;

    printf("Enter Stage 1 Pass: ");
    part1();
    printf("Stage 1 Defused\n");

    printf("Enter Stage 2 Pass: ");
    result = part2();
    printf("Stage 2 Defused\n");

    printf("Enter Stage 3 Pass: ");
    part3();
    printf("Stage 3 Defused\n");

    printf("Enter Stage 4 Pass: ");
    part4(result);
    printf("Stage 4 Defused\n");

    printf("Enter Stage 5 Pass: ");
    part5(buffer, bufSize);
    printf("Stage 5 Defused\n");

    printf("Enter Stage 6 Pass: ");
    part6(buffer);
    printf("Stage 6 Defused\n");

    printf("Enter Stage 7 Pass: ");
    part7(&result);
    printf("Stage 7 Defused\n");

    printf("Defusal complete");

}