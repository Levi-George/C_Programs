#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int MAX_MEALS =
    10; // maximum number of meals each philosopher may partake in
const int NUM_OF_PHIL = 5;//total number of philosophers
const double MAX_ACTIVE_TIME = 4;//time we can sleep, eat for

//enum for philosopher status and array of philosopher statuses
enum PHIL_STATUS { EATING, THINKING, HUNGRY };
enum PHIL_STATUS P_Status[5];

sem_t chopstick[5]; // chopsticks for eating, semaphores for food access
int totalMeals[5];  // times each philosopher eats
pthread_t philThreads[5]; 

int meals = 0; // have we reached 10 meals?
int philNum[5]; //stable memory for each philosopher ID

//mutex and condition variable
pthread_mutex_t P_StatusMutex;
pthread_cond_t P_StatusCond[5];

void *philosopher(void *arg);
void pickup_forks(void *);
void return_forks(void *);

int main(int argc, char *argv[]) {

  //debug message
  printf("Main Start\n");

  //seed random number generator
  srand(time(0));

  // argument checks
  if (argc != 2) {
    printf("You need one and only one argument\n");
    return 0;
  }

  //debug message
  printf("Check arg\n");

  // argument validity
  if (atoi(argv[1]) < 0) {
    printf("You cannot run the program for less than 0 seconds\n");
  }

  // argument conversion
  double time_Param = atof(argv[1]);

  // debug code
  printf("Dining Philosophers, running sim for: %f s\n", time_Param);
  printf("Before initialization of sem and mutex\n");

  // initialize our mutex and condition variable
  pthread_mutex_init(&P_StatusMutex, NULL);

  // debug message
  printf("After Mutex and Semaphore initialization\n");

  // initialize our semaphores
  for (int i = 0; i < NUM_OF_PHIL; i++) {
    sem_init(&chopstick[i], 0, 1);

    pthread_cond_init(&P_StatusCond[i], NULL);
  }

  //debug message
  printf("Before Thread Creation\n");

  // create 5 philosopher threads
  for (int i = 0; i < NUM_OF_PHIL; i++) {
    philNum[i] = i;
    // create thread
    // send it a proper memory address of our array of ints, we send a void
    // pointer of our memory address for each integer
    pthread_create(&philThreads[i], NULL, &philosopher,
                   (void *)&philNum[i]); // what var do we send
  }

  // sleep for time_Param
  sleep(time_Param);
  printf("***********************Time is up, join threads\n");

  // join threads back to main
  for (int i = 0; i < NUM_OF_PHIL; i++) {
    pthread_cancel(philThreads[i]);
  }

  // release our semaphores and get each philosopher's meals had
  printf("Meals had: *************\n");

  // data reclamation
  for (int i = 0; i < NUM_OF_PHIL; i++) {
    printf("Phil. %d : %d meals \n", i, totalMeals[i]);
    sem_destroy(&chopstick[i]);
  }

  return 0;
}

// Philosopher function
//INPUT:    Pointer to Philosopher Number
//PROCESS:  initiate philosopher functions, sleep and ouput progress, repeat until 
//          thread cancels or meals > 10
//OUTPUT:   no return values
void *philosopher(void *PN) {
  // we are passing the memory address of an array pointer
  // we need to get the integer pointer and dereference it
  int PHIL_NUM = *(int *)PN;

  while (totalMeals[PHIL_NUM] < MAX_MEALS) {

    printf("Philosopher %d preparing to eat\n", PHIL_NUM);
    pickup_forks(PN);

    // spend time eating
    sleep((rand() % (int)(MAX_ACTIVE_TIME)) +
          1); // sleep between 1 and 4 seconds

    printf("Philosopher %d preparing to think\n", PHIL_NUM);
    return_forks(PN);

    // spend time thinking
    sleep((rand() % (int)(MAX_ACTIVE_TIME)) +
          1); // sleep between 1 and 4 seconds

    // output code
    printf("Philosopher %d see meals eaten thus far: %d \n", PHIL_NUM,
           totalMeals[PHIL_NUM]);
  }

}

//INPUT:    Pointer to Philosopher Number
//PROCESS:  lock out code, update philosopher status, wait on other philosopher
//          for both chopsticks, attempt to get chopsticks, update to eating
//          unlock code
//OUTPUT:   no return values
void pickup_forks(void *PN) {
  
  //convert param to int
  int PHIL_NUM = *(int *)PN;

  //lock code
  printf("Philosopher %d locking out for chopstick pickup\n", PHIL_NUM);
  pthread_mutex_lock(&P_StatusMutex);

  //update philosopher status
  printf("Philosopher %d is now hungry\n", PHIL_NUM);
  P_Status[PHIL_NUM] = HUNGRY;

  // what about PHIL 0? We do this to avoid out-of-bounds when PHIL 0
  //  grabs it's left then right, stick
  int rightPHIL = (PHIL_NUM + 4) % NUM_OF_PHIL;
  int leftPHIL = (PHIL_NUM + 1) % NUM_OF_PHIL;

  //wait for left first, since we pickup chopsticks in the order of LEFT -> RIGHT
  printf("Philosopher %d checking if left neighbor is eating\n", PHIL_NUM);
  if (P_Status[leftPHIL] == EATING) {
    pthread_cond_wait(&P_StatusCond[leftPHIL], &P_StatusMutex);
  }
  printf("Philosopher %d checking if right neighbor is eating\n", PHIL_NUM);
  if (P_Status[rightPHIL] == EATING) {
    pthread_cond_wait(&P_StatusCond[rightPHIL], &P_StatusMutex);
  }

  //wait on chopstick semaphores to be posted
  printf("Philosopher %d waiting for chopsticks\n", PHIL_NUM);
  sem_wait(&chopstick[leftPHIL]);
  sem_wait(&chopstick[rightPHIL]);
  P_Status[PHIL_NUM] = EATING;//we eat once we have the semaphores

  //unlock mutex now that we are done 
  printf("Philosopher %d unlocking status mutex after fork pickup\n", PHIL_NUM);
  pthread_mutex_unlock(&P_StatusMutex);
}

//INPUT:    Pointer to Philosopher Number
//PROCESS:  surrender chopstick semaphores, lock out code for status updates
//          update total meals once philosopher goes from eating to thinking
//          unlock code 
//OUTPUT:   no return values
void return_forks(void *PN) {
  int PHIL_NUM = *(int *)PN;

  // what about PHIL 0? We do this to avoid out-of-bounds when PHIL 0
  //  grabs it's left then right, stick
  int rightPHIL = (PHIL_NUM + 4) % NUM_OF_PHIL;
  int leftPHIL = (PHIL_NUM + 1) % NUM_OF_PHIL;

  // release chopsticks to prevent deadlocking before locking out mutex
  printf("Philosopher %d releasing chopstick semaphores\n", PHIL_NUM);
  sem_post(&chopstick[rightPHIL]);
  sem_post(&chopstick[leftPHIL]);

  // lock out the mutex for status editing
  printf("Philosopher %d locking status mutex for fork return\n", PHIL_NUM);
  pthread_mutex_lock(&P_StatusMutex);

  // edit status of current philosopher then signal
  printf("Philosopher %d now beginning to think/release any status waiters\n",
         PHIL_NUM);
  P_Status[PHIL_NUM] = THINKING;
  pthread_cond_signal(&P_StatusCond[PHIL_NUM]);//now that we are done eating we update the condition variable

  //update our philosopher's meal count
  totalMeals[PHIL_NUM]++;

  // unlock mutex
  printf("Philosopher %d unlocking status mutex\n", PHIL_NUM);
  pthread_mutex_unlock(&P_StatusMutex);
}
