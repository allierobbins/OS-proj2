#include <stdio.h>
#include <unistd.h>

void producerFunction(int, pid_t);
void consumerFunction(int, pid_t);

int main(void) {
  pid_t  consumerpid, producerpid;      //ID for consumer and producer

  producerpid = fork();           //fork producer child

      if(producerpid == -1) {           //check that P fork successful
        perror("Failed to fork");
        return 1; }

      if (producerpid == 0) {
        producerFunction(20, producerpid);   //where code for producer goes
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
