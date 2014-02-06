#include<stdio.h>
#include<gtthread.h>

#define NO_OF_PHILOSOPHERS 5

typedef enum philState
{
    EATING,
    HUNGRY,
    THINKING
}state;


gtthread_mutex_t* chopstickLock[NO_OF_PHILOSOPHERS];
gtthread_mutex_t* monitor;
state philosopherState[NO_OF_PHILOSOPHERS];

void checkIfCanEat(int i);
void perform(int i);
void attemptToEat(int i);
void giveUpForks(int i);


/**
 * Philosopher ID
 */
void perform(int i)
{
    while(1)
    {
        //TO EMULATE THINKING
        fprintf(stderr, "\nPhilosopher %d is now thinking", i);
        sleep(5);
        
        //TRY TO  PICK CHOPSTICKS
        fprintf(stderr, "\nPhilosopher %d is now hungry", i);
      
        attemptToEat(i);

        fprintf(stderr, "\nPhilosopher %d is now eating", i);

        sleep(3); //Assuming he takes 3 seconds to eating

        giveUpForks(i);

        //Philosopher resumes thinking
     }

}

void attemptToEat(int i)
{
    gtthread_mutex_lock(monitor);
   
    philosopherState[i] = HUNGRY;

    checkIfCanEat(i);

    gtthread_mutex_unlock(monitor);

    gtthread_mutex_lock(chopstickLock[i]);
}

void giveUpForks(int i)
{
    int left, right;
    gtthread_mutex_lock(monitor);
      
    philosopherState[i] = THINKING;

    left = (i + NO_OF_PHILOSOPHERS - 1)% NO_OF_PHILOSOPHERS;
    right = (i) % NO_OF_PHILOSOPHERS;

    checkIfCanEat(left);
    checkIfCanEat(right);

    gtthread_mutex_unlock(monitor);  
}

void checkIfCanEat(int i)
{
    int left, right;


     right =(i) % NO_OF_PHILOSOPHERS;
     left = (i + NO_OF_PHILOSOPHERS - 1)% NO_OF_PHILOSOPHERS;

    if(philosopherState[i] == HUNGRY  && philosopherState[left] != EATING && philosopherState[right] != EATING)
    {
        philosopherState[i] == EATING;
        gtthread_mutex_unlock(chopstickLock[i]);
    }

}


int main()
{
    gtthread_t* threads[NO_OF_PHILOSOPHERS];
    int i = 0;

    for(i = 0; i < NO_OF_PHILOSOPHERS; i++)
    {
        threads[i] = (gtthread_t*)malloc(sizeof(gtthread_t));
        gtthread_create(threads[i], &perform, (void*)&i);
    }

    return 0;
}

