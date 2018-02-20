#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#define PERM (S_IRUSR | S_IWUSR)

int consumerStatus(pid_t [], int);      // Prototypes
void endProgram();
int detachAndRemove();

struct SharedSpace {     // Struct for shared memory
	int sharedVar;
	int turn;
	int flag[];
	int consumerProcesses;
	int processes;
};

int shmid;        // Global variables
struct SharedSpace *SM;

int main(int argc, char* argv[]) {

	signal(SIGINT, endProgram);     // Signal handler

	const char *PATH = "./consumer";

	int totalProcesses = 0;      // Local variables for index, max# of processes, timer,
	int	index = 0,
	int	timer = 100;

    time_t startTime = time(NULL);      // Creating time variables to use timer
    time_t currentTime = time(NULL);

    pid_t consumer, producer, consumerList[consumerProcesses];     // Process IDs for children

    if ((id = shmget(IPC_PRIVATE, sizeof(int), PERM)) == -1) {    // Create shared memory
      perror ("Failed to create shared memory segment");      // Failed to create shared memory
      return 1;
    }

    if ((sharedtotal = (int *)shmat(id, NULL, 0)) == (void *)-1) {      // Attach self shared memory
      perror("Failed to attach shared memory segment");       // Failed to attach self to shared memory

      if (shmctl(id, IPC_RMID, NULL) == -1)       // Detach shared memory
        perror("Failed to remove memory segment");      // Failed to detach shared memory
        return 1;
    }

	SM.sharedVar = 0;      // Writing to shared memory
	SM.consumerProcesses =  5;
	SM.turn = 0;

	do {     // Timer to execute while under 100 seconds

    producer = fork();

    if (producer == 0) {
        execl("./producer", id, (char *)NULL);   //where code for producer goes
        *sharedtotal = totalBytes;
        return 1; }

		if(totalProcesses < 20) {
			if(index < consumerProcesses) {      // Fork 20 processes
				pid = fork();
				consumerList[index] = pid;     // Keep record of child-consumer PIDs
				totalProcesses++;     // Increace # of processes
				index++;      // Increase index
			}

			if(conusmer == -1 || producer == -1) {      // If error forking children
				perror("Error when forking children.\n");

        if (detachAndRemove() == -1) {     // Detach shared memory
        perror("Error when detaching shared memory.\n");      // Error if failed to detach shared memory
        return 1;
        }
				exit(1);

			} else if (pid == 0) {      // Execute consumer code processes
				char *id;
				printf(id, "%i", index);      // Print PID and index
				Sm.processes++;     // Write to shared memory - increase processes

				execl("./consumer", id, (char *)NULL);

				exit(1);      // If consumer program execl fails - exit

			}

			startTime = time(NULL);     // Reset time

		} else {
			printf(stderr, "Too many processes running - killing all and ending program.\n");    // If lots of processes are running - exit
			endProgram(2);
			exit(1);
		}
  } while((startTime - currentTime) < timer);

	}

	if((wait(NULL) >= 0) {      // 100 seconds have pass
		perror("Error in completing all processes in 100 seconds.\n");      //If consumers running - kill all processes & error
		endProgram();
	} else {
    if (detachAndRemove() == -1) {     // Detach shared memory
    perror("Failed to destroy shared memory segment");      // Error if failed ot detach shared memory
    return 1;
    }

		printf("Finished %i logs for %i processes.\n", 20, consumerProcesses);
	}
}

int consumerStatus(pid_t consumerList[], int size) {       // Checks if consumers are running
	int i, status;     //variables

	for(i = 0; i < size; i++) {      // Check PIDs status of consumer
		pid_t wid = waitpid(consumerList[c], &status, WNOHANG);

		if(wid != 0) {
			consumerList[i] = 0;
		}
  }

	for(i = 0; i < size; i++) {
		if(consumers[i] == 0)     // If process is running - continue
			continue;
		else      // Process is done - return 0
			return 0;
	}
		return 1;
}

void endProgram() {      // Kills all when signal is received
    pid_t id = getpgrp();
	printf("Signal received - exiting master program with PID: %i.\n", id);

  if (detachAndRemove() == -1) {
      perror("Failed to destroy shared memory segment");
      return 1;
    }
      //kills all processes running
    killpg(id, SIGINT);
    exit(EXIT_SUCCESS);
}

int detachAndRemove() {

  if (shmdt(num) && shmdt(turn) && shmdt(flag) && shmctl(sharedMem, IPC_RMID, NULL))
    return 1;
  else
    return -1;
}
