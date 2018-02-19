#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#define  PERM (S_IRUSR | S_IWUSR)

void producerFunction(int, pid_t);
void consumerFunction(int, pid_t);
int detachAndRemove(int shmid, void *shmaddr);

int main(void) {
  pid_t  consumerpid, producerpid;      //ID for consumer and producer
  int id;                               //For shared memory
  int *sharedtotal;                     //For shared memory
  int totalBytes = 0;

  if ((id = shmget(IPC_PRIVATE, sizeof(int), PERM)) == -1) {
    perror ("Failed to create shared memory segment");
    return 1;
  }
  if ((sharedtotal = (int *)shmat(id, NULL, 0)) == (void *)-1) {
    perror("Failed to attach shared memory segment");
    if (shmctl(id, IPC_RMID, NULL) == -1)
      perror("Failed to remove memory segment");
      return 1;
  }

  producerpid = fork();           //fork producer child

      if(producerpid == -1) {           //check that P fork successful
        perror("Failed to fork");
        if (detachAndRemove(id, sharedtotal) == -1)
          perror("Failed to destroy shared memory segment");
        return 1; }

      if (producerpid == 0) {
        producerFunction(20, producerpid);   //where code for producer goes
        *sharedtotal = totalBytes;
        return 1; }

  consumerpid = fork();         //fork consumer child

      if(consumerpid == -1) {        //check that C fork successful
        perror("Failed to fork");
        return 1; }

      if (consumerpid == 0) {
        consumerFunction(5, consumerpid);    //where code for consumer goes
        return 1; }

  if (consumerpid != wait(NULL) && producerpid != wait(NULL)) {
      printf("\nI am the master parent.\n");
      printf("My PID is %ld.\n", (long)getpid());     //where code for master goes
      return 1;
    }

  if (detachAndRemove(id, sharedtotal) == -1) {
    perror("Failed to destroy shared memory segment");
    return 1;
  }

return 0;
}

void producerFunction(int max, pid_t pid) {

    printf("\n In the producerFunction.\n");
    printf("My PID is %ld and I can have %d children.\n", (long)getpid(), max);
}

void consumerFunction(int max, pid_t pid) {

    printf("\n In the consumerFunction.\n");
    printf("My PID is %ld and I can have %d children.\n", (long)getpid(), max);
}

int detachAndRemove(int shmid, void *shmaddr) {
  int error = 0;

  if (shmdt(shmaddr) == -1)
    error = errno;
  if ((shmctl(shmid, IPC_RMID, NULL) == -1) && !error)
    error = errno;
  if (!error)
    return 0;
    errno = error;
    return -1;
}
