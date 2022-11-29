/**
 * fifteen.c
 *
 * Refer to Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

#ifndef INT_MAX
    #define INT_MAX 12345678
#endif // INT_MAX

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void greet(void);
void init(void);
void draw(void);
short move(int tile);
short won(void);
int get_int();

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (1)
    {
        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        printf("\n");

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Get an non-negative integer from user input
 * If the input is not non-negative integer, return INT_MAX
 */
 int get_int()
 {
    int input = 0;
    short invalid = 0;

    char c = getchar();
    if (c == '\n')
        return INT_MAX;

    while (c != '\n') 
    {
        if ((c >= '0') && ( c <= '9'))
        {
            input = input * 10 + (c - '0');
        } else {
            invalid = 1;
        }

        c = getchar();
    }

    if (invalid)
        return INT_MAX;
    else 
        return input;
 }

/**
 * Greets player.
 */
void greet(void)
{    
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    // TODO

    /* My understanding is that the top left should be the largest number
    Thus, I make the first number the largest number, here is how I visualized the board based on our established indices

    X X X X X X X X X
    X X X X X X X X X
    X X X X X X X X X
    X X X X X X X X X
    X X X X X X X X X
    X X X X X X X X X
    X X X X X X X X X
    X X X X X X X X X
    X X X X X X X X X

    Thus, I would get something like this for the 4 by 4 board.

    15 14 13 12 X X X X X
    11 10 9  8  X X X X X
    7  6  5  4  X X X X X
    3  1  2  _  X X X X X
    X  X  X  X  X X X X X
    X  X  X  X  X X X X X
    X  X  X  X  X X X X X
    X  X  X  X  X X X X X
    X  X  X  X  X X X X X    

    */

    int count = (d * d) - 1; //using this just makes it mentally 
    //easier for me to initialize this board

    int maxNum = (d*d)-1;

    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {

            //this structure performs the switch between 1 and 2 that is present in boards with an odd number of 
            //tiles
            if(count == 2 && maxNum % 2)//odd tiled behavior
            {
                //finishes the board and swaps the 2 and 1
                board[i][j+2] = 0;
                board[i][j+1] = 2;
                board[i][j] = 1;

                j = d; //this ends the inner J-loop
            }
            else//even tiled behavior
            {
                board[i][j] = count;

                count--;
            }
            

            
        }
    }

    //End - TODO
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    //TODO
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            //prints _ if the current slot is 0
            if(board[i][j] == 0)
            {
                printf(" _ ");
            }
            else//prints value in slot
            {
                printf("%2d ", board[i][j]);
            }
            
            
        }

        printf("\n");
    }
    //End - TODO
}

/**
 * If tile borders empty space, moves tile and returns 1, else
 * returns 0.
 */
short move(int tile)
{
    // TODO

    //The I and J for-loops iterate through the board searching for our selected tile
    //When the tile is found it will check around the tile for 0, but carefully to avoid going Out-of-Bounds
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {

            //for each IF statement, we can't go left (right, up, down) without potentially going Out-of-Bounds
            //so we check where our count is, if it precludes a specific direction then we exclude it from our search

            if(board[i][j] == tile && j != 0)//check if left is safe from Out-of-Bounds 
            {
                //if the location to the left is 0,
                if(board[i][j-1] == 0)
                {

                    //we then swap 0 and our tile number.
                    board[i][j] = 0;
                    board[i][j-1] = tile;

                    return 1;
                }
            }//End - IF

            if(board[i][j] == tile && j != (d-1))//check right
            {
                if(board[i][j+1] == 0)
                {
                    board[i][j] = 0;
                    board[i][j+1] = tile;

                    return 1;
                }
            }//End - IF

            if(board[i][j] == tile && i != 0)//check top
            {
                if(board[i-1][j] == 0)
                {
                    board[i][j] = 0;
                    board[i-1][j] = tile;

                    return 1;
                }
            }//End - IF

            if(board[i][j] == tile && i != (d-1))//check bottom
            {
                if(board[i+1][j] == 0)
                {
                    board[i][j] = 0;
                    board[i+1][j] = tile;

                    return 1;
                }
            }//End - IF

        }//End - FOR (J)
    }//End - FOR (I)

    return 0;
}//End move()

/**
 * Returns 1 if game is won (i.e., board is in winning configuration),
 * else 0.
 */
short won(void)
{
    // TODO
    //The logic behind this is that as the loops iterate through
    //each part of the board, it will only ever count sequentially while moving through a winning board
    //so for each position, it wll be the previous position + 1. Thus, a winning board could
    //be counted through just by adding 1 to the value of the first number in the first position
    //then moving to the next
    
    int count = 0;

    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++) //d - 1 since we do not want to check the last position for a winning board
        {
            if(count + 1 == board[i][j] )//checks start of board, does count + 1, because count = 1 would cause the final count to be 9
            {//It accounts for the empty tile
                count++;
            }
            
            
        }//End - For(j)

    }//End - For(i)

    if((d * d - 1) == board[d-1][d - 2] && count == (d*d - 1))//will only succeed if (d*d - 1) is in the second to last slot on the board
    {//and the count is d * d - 1

     //It is done this way in order to avoid instances where the count adds up to (d * d -1) 
     //but that value is not in the second to last position

        return 1;
    }
    else
    {
        return 0;
    }
    

    return 1;
}
