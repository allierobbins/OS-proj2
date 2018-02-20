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

int consumerChecker(pid_t [], int, int);      // Prototypes
void signalHandler();
void killAll();

struct SharedMemory {     // Struct for shared memory
	int sharedVar;
	int turn;
	int flag[19];
	int maxWrites;
	int consumerProcesses;
	int processes;
};

int shmid;        // Global variables
struct SharedMemory *shmem;
key_t key = 2018;

int main(int argc, char* argv[]) {

	signal(SIGINT, signalHandler);     // Signal handler

	const char *PATH = "./consumer";

	int op, totalProcesses,      // Local variables for index, max# of processes, timer,
		index = 0,
		maxWrites = 3,
		consumerProcesses = 5,
		timer = 100;

	if((shmid = shmget(key, sizeof(struct SharedMemory) * 2, IPC_CREAT | 0666)) < 0) {     // Createing shared memory ID
		perror("shmget");
		printf(stderr, "Error creating shraed memory - ending program.\n");     // If failed to create shared memory ID
		exit(EXIT_FAILURE);
	}

    if ((shmem = (struct SharedMemory *)shmat(shmid, NULL, 0)) == (struct SharedMemory *) -1) {     	// Attaching shared memory
		perror("shmat");
        printf(stderr, "Error attaching shraed memory - ending program.\n");      // If failed to attach shared memory
        exit(EXIT_FAILURE);
    }

	time_t t1 = time(NULL);      // Creating time variables to use timer
	time_t t2 = time(NULL);

	pid_t pid, consumers[consumerProcesses];     // Process IDs for child-consumers

	shmem->maxWrites = maxWrites;      // Writing to shared memory
	shmem->sharedVar = 0;
	shmem->consumerProcesses = consumerProcesses + 1;
	shmem->turn = 0;

	while((t2 - t1) < timer) {     // Timer to execute while under 100 seconds
		if(totalProcesses < 20) {
			if(index < consumerProcesses) {      // Fork 20 processes
				pid = fork();
				consumers[index] = pid;     // Keep record of child-consumer PIDs
				totalProcesses++;     // Increace # of processes
				index++;      // Increase index
			}

			if(pid == -1) {      // If error forking child-consumers
				printf(stderr, "Error when forking consumers - ending program.\n");
				killAll();      // End all processes & exit
				exit(EXIT_FAILURE);

			} else if(pid == 0) {      // Execute consumer code processes
				char *id;
				printf(id, "%i", index);      // Print PID and index
				shmem->processes++;     // Write to shared memory - increase processes

				execl(PATH, id, (char *)NULL);

				_exit(EXIT_FAILURE);      // If consumer program execl fails - exit

			}

			t2 = time(NULL);     // Reset time

		} else {
			printf(stderr, "There are too many processes running - killing all and ending program.\n");    // If lots of processes are running - exit
			signalHandler(2);
			exit(EXIT_FAILURE);
		}

		int check = consumerChecker(consumers, consumerProcesses, maxWrites);
		if(check == 1) {      	// Check consumer processes status - If there's consumers running then continue - or let master finish
			break;
		}

	}

	if((wait(NULL) > 0) && (shmem->processes != 0)) {      // If 100 seconds pass - check that consumers are running
		printf(stderr,"Error in completing all processes in 100 seconds - Killing all processes.\n");      //If consumers running - kill all processes & error
		signalHandler();
	} else {     	//end successfully
		killAll();
		printf("Completed %i writes for %i processes.\n", maxWrites, consumerProcesses);
	}
}

int consumerChecker(pid_t consumers[], int size, int maxWrites) {       // Checks if consumers are running
	int c, status;     //variables

	for(c = 0; c < size; c++) {      //check PIDs of consumer process
		pid_t wid;
		wid = waitpid(consumers[c], &status, WNOHANG);

		if(wid != 0) {
			consumers[c] = 0;
		}
	}

	for(c = 0; c < size; c++) {
		if(consumers[c] == 0)     //if process is running - continue
			continue;
		else      //process is done - return
			return 0;
	}
		return 1;
}

void signalHandler() {      // Kills all when signal is received
    pid_t id = getpgrp();
	printf("Signal received - exiting master program with PID: %i.\n", id);
	killAll();     //kills all processes running
    killpg(id, SIGINT);
    exit(EXIT_SUCCESS);
}

void killAll() {      // detaches all shared memory
	shmctl(shmid, IPC_RMID, NULL);
	shmdt(shmem);
}
