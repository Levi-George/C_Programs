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

int meals = 0; // have we reached 10 meals?
sem_t mealAccess;

pthread_mutex_t P_StatusMutex;
pthread_cond_t P_StatusCond[5];


void *philosopher(void *arg);
void pickup_forks(int*);
void return_forks(int*);


int main(int argc, char * argv[])
{
    srand(time(0));

    if(argc != 2)
    {
        printf("You need one and only one argument\n");
        return 0;
    }

    if(argv[2] < 0 )
    {
        printf("You cannot run the program for less than 0 seconds\n");
    }

    int time_Param = atof(argv[2]);

    //initialize our mutex and condition variable
    pthread_mutex_init(&P_StatusMutex, NULL);
    sem_init(&mealAccess, 0, 1);

    //initialize our semaphores
    for(int i = 0; i < NUM_OF_PHIL; i++)
    {
        sem_init(&chopstick[i], 0, 1);
        
        pthread_cond_init(&P_StatusCond[i], NULL);
    }
    

    //create 5 philosopher threads
    for(int i = 0; i < NUM_OF_PHIL; i++)
    {
        pthread_create(&philThreads[i],NULL,&philosopher,&i); //what var do we send
    }

    //sleep for time_Param
    sleep(time_Param);

    //join threads back to main
    for(int i = 0; i < NUM_OF_PHIL; i++)
    {
        pthread_join(philThreads[i],NULL);
    }


    //release our semaphores and get each philosopher's meals had
    printf("Meals had: ");
    
    for(int i = 0; i < NUM_OF_PHIL; i++)
    {
        printf("Phil. %d : %d meals ",i,totalMeals[i]);
        sem_destroy(&chopstick[i]);
    }

    return 0;
}

//Philosopher functions

void *philosopher(void *PHIL_NUM)
{
    //is meals subject to alteration?
    while(meals < MAX_MEALS)
    {
        //after we check our while condition
        sem_post(&mealAccess);
        
        pickup_forks(PHIL_NUM);

        //spend time eating
        sleep((rand() % (int)(MAX_ACTIVE_TIME)) + 1); //sleep between 1 and 4 seconds

        return_forks(PHIL_NUM);
        
        //spend time thinking
        sleep((rand() % (int)(MAX_ACTIVE_TIME)) + 1); //sleep between 1 and 4 seconds

        //before we check our while condition
        sem_wait(&mealAccess);
    }


}

void pickup_forks(int* PHIL_NUM)
{
    pthread_mutex_lock(&P_StatusMutex);

    P_Status[*PHIL_NUM] = HUNGRY;

    //what about PHIL 0? We do this to avoid out-of-bounds when PHIL 0
    //  grabs it's left then right, stick
    int rightPHIL = (*PHIL_NUM + 4) % NUM_OF_PHIL;
    int leftPHIL = (*PHIL_NUM + 1) % NUM_OF_PHIL;

    if(P_Status[rightPHIL] == EATING || P_Status[leftPHIL] == EATING)
    {
        //we wait for the left first then the second
        pthread_cond_wait(&P_StatusCond[leftPHIL], &P_StatusMutex);
        pthread_cond_wait(&P_StatusCond[rightPHIL], &P_StatusMutex);
    }

    sem_wait(&chopstick[rightPHIL]);
    sem_wait(&chopstick[leftPHIL]);
    P_Status[*PHIL_NUM] = EATING;

    pthread_mutex_unlock(&P_StatusMutex);

}
    


void return_forks(int *PHIL_NUM)
{
    //what about PHIL 0? We do this to avoid out-of-bounds when PHIL 0
    //  grabs it's left then right, stick
    int rightPHIL = (*PHIL_NUM + 4) % NUM_OF_PHIL;
    int leftPHIL = (*PHIL_NUM + 1) % NUM_OF_PHIL;

    pthread_mutex_lock(&P_StatusMutex); 

    sem_post(&chopstick[rightPHIL]);
    sem_post(&chopstick[leftPHIL]);

    P_Status[*PHIL_NUM] = THINKING;
    pthread_cond_signal(&P_StatusCond[*PHIL_NUM]);

    sem_wait(&mealAccess);
    meals++;
    totalMeals[*PHIL_NUM]++;
    sem_post(&mealAccess);

    pthread_mutex_unlock(&P_StatusMutex);
}

