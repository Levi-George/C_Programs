#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>


const int MAX_MEALS = 10; //maximum number of meals each philosopher may partake in
const int NUM_OF_PHIL = 5;
const double MAX_ACTIVE_TIME = 4;

enum PHIL_STATUS
{
    EATING, THINKING, WAITING
};

enum PHIL_STATUS Philosophers[5];
sem_t chopstick[5]; //chopsticks for eating, semaphores for food access
int totalMeals[5]; // times each philosopher eats
pthread_t philThreads[5];

int meals = 0;


int philosopher();
int pickup_forks(int, int);
int return_forks(int, int);


int main(double time_Param)
{
    srand(time(0));

    if(time_Param < 1)
    {
        printf("Yo, you should try to enter a positive amount of time ( > 0 )");
        return 1;
    }

    //initialize our semaphores
    for(int i = 0; i < 5; i++)
    {
        sem_init(&chopstick[i], 0, 1);
    }
    

    //create 5 philosopher threads
    for(int i = 0; i < 5; i++)
    {
        pthread_create(&philThreads[i],NULL,philosopher(i),NULL); //what var do we send
    }

    //sleep for time_Param
    sleep(time_Param);

    //join threads back to main
    for(int i = 0; i < 5; i++)
    {
        pthread_join(&philThreads[i],NULL);
    }


    //release our semaphores and get each philosopher's meals had
    printf("Meals had: ");
    
    for(int i = 0; i < 5; i++)
    {
        printf(" " + totalMeals[i]);
        sem_destroy(&chopstick[i]);
    }

    return 0;
}

//Philosopher functions

int philosopher(int PHIL_NUM)
{
    const int PHIL_NUM = PHIL_NUM;
    

    while(meals < MAX_MEALS)
    {
        //what about PHIL 0? We do this to avoid out-of-bounds when PHIL 0
        //  grabs it's left then right, stick
        int rightStick = (PHIL_NUM + 4) % 5;

        pickup_forks(PHIL_NUM, rightStick);

        //spend time eating
        sleep((rand() % (int)(MAX_ACTIVE_TIME)) + 1); //sleep between 1 and 4 seconds

        return_forks(PHIL_NUM, rightStick);

        //spend time thinking
        sleep((rand() % (int)(MAX_ACTIVE_TIME)) + 1); //sleep between 1 and 4 seconds

    }


    return meals;
}

int pickup_forks(int PHIL_NUM, int rightStick)
{
    //Wait for semaphore to left and right
    sem_wait(chopstick[PHIL_NUM]); //left stick
    sem_wait(chopstick[rightStick]); //right stick

    //change philosopher's status
    Philosophers[PHIL_NUM] = EATING;

    ++totalMeals[PHIL_NUM];
    meals++;
}
    

//Check if we are eating, before we sleep we need to return chopsticks before we sleep
int return_forks(int PHIL_NUM, int rightStick)
{
    sem_post(chopstick[PHIL_NUM]); //left stick
    sem_post(chopstick[rightStick]); //right stick

    //change status
    Philosophers[PHIL_NUM] = THINKING;

    
}

