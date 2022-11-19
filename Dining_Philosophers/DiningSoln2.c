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
int philNum[5];

pthread_mutex_t P_StatusMutex;
pthread_cond_t P_StatusCond[5];


void *philosopher(void *arg);
void pickup_forks(void*);
void return_forks(void*);


int main(int argc, char * argv[])
{
    printf("Main Start\n");

    srand(time(0));

    if(argc != 2)
    {
        printf("You need one and only one argument\n");
        return 0;
    }

    printf("Check arg\n");

    if(atoi(argv[1]) < 0 )
    {
        printf("You cannot run the program for less than 0 seconds\n");
    }

    double time_Param = atof(argv[1]);

    printf("Dining Philosophers, running sim for: %f s\n", time_Param);
    printf("Before initialization of sem and mutex\n");

    //initialize our mutex and condition variable
    pthread_mutex_init(&P_StatusMutex, NULL);
    sem_init(&mealAccess, 0, 1);

    printf("After Mutex and Semaphore initialization\n");

    //initialize our semaphores
    for(int i = 0; i < NUM_OF_PHIL; i++)
    {
        sem_init(&chopstick[i], 0, 1);
        
        pthread_cond_init(&P_StatusCond[i], NULL);
    }
    
    printf("Before Thread Creation\n");

    //create 5 philosopher threads
    for(int i = 0; i < NUM_OF_PHIL; i++)
    {
        philNum[i] = i;
        //create thread
        // send it a proper memory address of our array of ints, we send a void pointer
        // of our memory address for each integer
        pthread_create(&philThreads[i],NULL,&philosopher,(void *)&philNum[i]); //what var do we send
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
        printf("Phil. %d : %d meals \n",i,totalMeals[i]);
        sem_destroy(&chopstick[i]);
    }

    return 0;
}

//Philosopher functions

void *philosopher(void *PN)
{
    //we are passing the memory address of an array pointer
    // we need to get the integer pointer and dereference it
    int PHIL_NUM = *(int *)PN;

    //is meals subject to alteration?
    while(meals < MAX_MEALS)
    {
        //after we check our while condition
        sem_post(&mealAccess);
        
        printf("Philosopher %d preparing to eat\n", PHIL_NUM);
        pickup_forks(PN);

        //spend time eating
        sleep((rand() % (int)(MAX_ACTIVE_TIME)) + 1); //sleep between 1 and 4 seconds

        printf("Philosopher %d preparing to think\n", PHIL_NUM);
        return_forks(PN);
        
        //spend time thinking
        sleep((rand() % (int)(MAX_ACTIVE_TIME)) + 1); //sleep between 1 and 4 seconds

        //before we check our while condition
        printf("Philosopher %d checks meals\n", PHIL_NUM);
        sem_wait(&mealAccess);
        printf("Philosopher %d's meals eaten thus far: %d \n", PHIL_NUM,meals );
    }

    //we exit the loop then we release our semaphore
    sem_post(&mealAccess);

}

void pickup_forks(void* PN)
{
    int PHIL_NUM = *(int *)PN;
    
    printf("Philosopher %d locking out for chopstick pickup\n", PHIL_NUM);
    pthread_mutex_lock(&P_StatusMutex);

    printf("Philosopher %d is now hungry\n", PHIL_NUM);
    P_Status[PHIL_NUM] = HUNGRY;

    //what about PHIL 0? We do this to avoid out-of-bounds when PHIL 0
    //  grabs it's left then right, stick
    int rightPHIL = (PHIL_NUM + 4) % NUM_OF_PHIL;
    int leftPHIL = (PHIL_NUM + 1) % NUM_OF_PHIL;

    printf("Philosopher %d checking if left neighbor is eating\n", PHIL_NUM);
    if(P_Status[leftPHIL] == EATING)
    {
        //we wait for the left first then the second
        pthread_cond_wait(&P_StatusCond[leftPHIL], &P_StatusMutex);
       
    }
    printf("Philosopher %d checking if right neighbor is eating\n",PHIL_NUM);
    if(P_Status[rightPHIL] == EATING)
    {
         pthread_cond_wait(&P_StatusCond[rightPHIL], &P_StatusMutex);
    }

    printf("Philosopher %d waiting for chopsticks\n", PHIL_NUM);
    sem_wait(&chopstick[rightPHIL]);
    sem_wait(&chopstick[leftPHIL]);
    P_Status[PHIL_NUM] = EATING;

    printf("Philosopher %d unlocking status mutex after fork pickup\n", PHIL_NUM);
    pthread_mutex_unlock(&P_StatusMutex);

}
    


void return_forks(void *PN)
{
    int PHIL_NUM =*(int *)PN;

    //what about PHIL 0? We do this to avoid out-of-bounds when PHIL 0
    //  grabs it's left then right, stick
    int rightPHIL = (PHIL_NUM + 4) % NUM_OF_PHIL;
    int leftPHIL = (PHIL_NUM + 1) % NUM_OF_PHIL;

    printf("Philosopher %d releasing chopstick semaphores\n", PHIL_NUM);
    sem_post(&chopstick[rightPHIL]);
    sem_post(&chopstick[leftPHIL]);

    printf("Philosopher %d locking status mutex for fork return\n", PHIL_NUM);
    pthread_mutex_lock(&P_StatusMutex); 

    printf("Philosopher %d now beginning to think/release any status waiters\n", PHIL_NUM);
    P_Status[PHIL_NUM] = THINKING;
    pthread_cond_signal(&P_StatusCond[PHIL_NUM]);

    printf("Philosopher %d locking meal semaphore\n", PHIL_NUM);
    sem_wait(&mealAccess);
    meals++;
    totalMeals[PHIL_NUM]++;
    sem_post(&mealAccess);
    printf("Philosopher %d unlocking meal semaphore\n", PHIL_NUM);

    printf("Philosopher %d unlocking status mutex\n", PHIL_NUM);
    pthread_mutex_unlock(&P_StatusMutex);
}

