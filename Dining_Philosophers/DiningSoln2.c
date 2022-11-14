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
    EATING, THINKING, HUNGRY
};

enum PHIL_STATUS P_Status[5];

sem_t chopstick[5]; //chopsticks for eating, semaphores for food access
int totalMeals[5]; // times each philosopher eats
pthread_t philThreads[5];

int meals = 0;

pthread_mutex_t P_StatusMutex;
pthread_cond_t P_StatusCond[5];


int philosopher();
int pickup_forks(int);
int return_forks(int);


int main(double time_Param)
{
    srand(time(0));

    if(time_Param < 1)
    {
        printf("Yo, you should try to enter a positive amount of time ( > 0 )");
        return 1;
    }

    //initialize our mutex and condition variable
    pthread_mutex_init(&P_StatusMutex, NULL);

    //initialize our semaphores
    for(int i = 0; i < 5; i++)
    {
        sem_init(&chopstick[i], 0, 1);
        
        pthread_cond_init(&P_StatusCond[i], NULL);
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
    

    //is meals subject to alteration?
    while(meals < MAX_MEALS)
    {

        
        pickup_forks(PHIL_NUM);

        //spend time eating
        sleep((rand() % (int)(MAX_ACTIVE_TIME)) + 1); //sleep between 1 and 4 seconds

        return_forks(PHIL_NUM);
        
        //spend time thinking
        sleep((rand() % (int)(MAX_ACTIVE_TIME)) + 1); //sleep between 1 and 4 seconds


    }


    return meals;
}

int pickup_forks(int PHIL_NUM)
{
    pthread_mutex_lock(&P_StatusMutex);

    P_Status[PHIL_NUM] = HUNGRY;

    //what about PHIL 0? We do this to avoid out-of-bounds when PHIL 0
    //  grabs it's left then right, stick
    int rightPHIL = (PHIL_NUM + 4) % 5;
    int leftPHIL = (PHIL_NUM + 1) % 5;

    if(P_Status[rightPHIL] == EATING || P_Status[leftPHIL] == EATING)
    {
        //we wait for the left first then the second
        pthread_cond_wait(&P_StatusCond[leftPHIL], &P_StatusMutex);
        pthread_cond_wait(&P_StatusCond[rightPHIL], &P_StatusMutex);
    }

    sem_wait(&chopstick[rightPHIL]);
    sem_wait(&chopstick[leftPHIL]);
    P_Status[PHIL_NUM] = EATING;

    pthread_mutex_unlock(&P_StatusMutex);

}
    


int return_forks(int PHIL_NUM)
{
    //what about PHIL 0? We do this to avoid out-of-bounds when PHIL 0
    //  grabs it's left then right, stick
    int rightPHIL = (PHIL_NUM + 4) % 5;
    int leftPHIL = (PHIL_NUM + 1) % 5;

    pthread_mutex_lock(&P_StatusMutex); 

    sem_post(&chopstick[rightPHIL]);
    sem_post(&chopstick[leftPHIL]);

    P_Status[PHIL_NUM] = THINKING;
    pthread_cond_signal(&P_StatusCond[PHIL_NUM]);

    pthread_mutex_unlock(&P_StatusMutex);
}

