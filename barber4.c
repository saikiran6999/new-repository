#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> 
#define MAX_CUSTOMERS 3 

void *customer(void *num);
void *barber(void *);
void randwait(int secs);


sem_t waitingRoom;

sem_t barberChair;

sem_t barberPillow;

sem_t seatBelt;

int allDone = 0;

int main(int argc, char *argv[])
{
    pthread_t btid;
    pthread_t tid[MAX_CUSTOMERS];
    int i, x, numCustomers, numChairs; int Number[MAX_CUSTOMERS];
    printf("Maximum number of customers can only be 3. Enter number of Students and chairs.\n");
    scanf("%d",&x);
    numCustomers = x;
    scanf("%d",&x);
    numChairs = x;
    if (numCustomers > MAX_CUSTOMERS) {
       printf("The maximum number of Students is %d.\n", MAX_CUSTOMERS);
       system("PAUSE");   
       return 0;
    }
    printf("A solution to the sleeping TA problem using semaphores.\n");
    for (i = 0; i < MAX_CUSTOMERS; i++) {
        Number[i] = i;
    }
    // Initialize the semaphores with initial values...
    sem_init(&waitingRoom, 0, numChairs);
    sem_init(&barberChair, 0, 1);
    sem_init(&barberPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);
   
    // Create the barber.
    pthread_create(&btid, NULL, barber, NULL);
   
    // Create the customers.
    for (i = 0; i < numCustomers; i++) {
        pthread_create(&tid[i], NULL, customer, (void *)&Number[i]);
    }
    // Join each of the threads to wait for them to finish.
    for (i = 0; i < numCustomers; i++) {
        pthread_join(tid[i],NULL);
    }
    // When all of the customers are finished, kill the barber thread.
    allDone = 1;
    sem_post(&barberPillow); // Wake the barber so he will exit.
    pthread_join(btid,NULL);
    system("PAUSE");   
    return 0;
}

void *customer(void *number) {
     int num = *(int *)number; // Leave for the shop and take some random amount of  time to arrive.
     printf("Student %d leaving for TA office.\n", num);
     randwait(5);
     printf("Student %d arrived at TA office.\n", num); // Wait for space to open up in the waiting room...
     sem_wait(&waitingRoom);
     printf("Student %d entering waiting room.\n", num); // Wait for the barber chair to become free.
     sem_wait(&barberChair); // The chair is free so give up your spot in the  waiting room.
     sem_post(&waitingRoom); // Wake up the barber...
     printf("Student %d waking the TA.\n", num);
     sem_post(&barberPillow); // Wait for the barber to finish cutting your hair.
     sem_wait(&seatBelt); // Give up the chair.
     sem_post(&barberChair);
     printf("Student %d leaving TA office.\n", num);
}

void *barber(void *junk)
{
// While there are still customers to be serviced... Our barber is omnicient and can tell if there are  customers still on the way to his shop.
  
  while (!allDone) { // Sleep until someone arrives and wakes you..
    printf("The TA is sleeping\n");
    sem_wait(&barberPillow); // Skip this stuff at the end...
    if (!allDone)
    { // Take a random amount of time to cut the customer's hair.
     printf("The TA is cutting hair\n");
     randwait(3);
     printf("The TA has finished cutting hair.\n"); // Release the customer when done cutting...
     sem_post(&seatBelt);
    }
    else {
         printf("The TA is going home for the day.\n");
    }
   }
}
   
void randwait(int secs) {
     int len = 1; // Generate an arbit number...
     sleep(len);
}

