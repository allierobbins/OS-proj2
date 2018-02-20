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

void signalHandler();     // Prototypes
void killAll();

struct SharedMemory {     // Struct for shared memory
	int sharedVar;
	int turn;
	int flag[19];
	int maxWrites;
	int consumerProcesses;
	int processes;
};

enum state {waiting, wantInCS, inCriticalSection};      // States for CS flags

int shmid;      //  Global variables
struct SharedMemory *shmem;
key_t key = 2018;

int main(int argc, char* argv[]) {

	signal(SIGINT, signalHandler);     // Signal Handler

	int i = atoi(argv[0]);       // Get passed arguments from execl
	logFile = argv[1];

	if((shmid = shmget(key, sizeof(struct SharedMemory) * 2, 0666)) < 0) {     // Get shared memory id from master process
		perror("shmget");                                                         // If no shared memory id is caught - eexit
		printf(stderr, "Error getting shared memory id - ending program.\n");
		exit(EXIT_FAILURE);
	}

    if ((shmem = (struct SharedMemory *)shmat(shmid, NULL, 0)) == (struct SharedMemory *) -1) {     	// Attach shared memory to consumer process
		perror("shmat");
        printf(stderr, "Error attaching shared memory - ending program.\n");      // Error message if shared memory is not attached & exit
        exit(EXIT_FAILURE);
    }

	int a, b, c = shmem->consumerProcesses;      // Algorithm for CS entry
	for(a = 0; a < shmem->maxWrites; a++) {
		do {
			shmem->flag[i] = wantInCS;     // Set flags to 'wanting in CS'

			b = shmem->turn;     // Assignment

			while(b != i) {
				b = (shmem->flag[b] != waiting) ? shmem->turn : (b + 1) % c;      // Set flag to 'waiting'
			}

			shmem->flag[i] = inCriticalSection;      // Declare wanting to enter CS

			for(b = 0; b < c; b++) {
				if((b != i) && (shmem->flag[b] == inCriticalSection)) {     // See if anyone else is in CS
					break;
				}
			}
		} while((b < c) || ((shmem->turn != i) && (shmem->flag[shmem->turn] != waiting)));

		shmem->turn = i;      // Assign to self - go into CS


		printf(stderr, "Child PID: %i - Entering Critical Section.\n", getpid());      //CS

		struct timespec tm;     // Setting up for random # between 0 - 5
		clock_gettime(CLOCK_MONOTONIC, &tm);
		srand((unsigned)(tm.tv_sec ^ tm.tv_nsec ^ (tm.tv_nsec >> 30)));
		sleep((rand() % 5));        		// Sleep for 0 - 5 seconds

		char *t = malloc(10);
		time_t theTime;
		theTime = time(NULL);                 // Getting time in HH:MM:SS for log
		struct tm *p = localtime(&theTime);
		strftime(t, 10, "%H:%M:%S", p);

		shmem->sharedVar++;     		// Increase sharedVar in shared memory

		printf("\tFile modified by process number %i at time %s with sharedNum = %d.\n", i, t, shmem->sharedVar);      // Print log message

		sleep((rand() % 5));      	// Sleep for 0 - 5 seconds


		b = (shmem->turn + 1) % c;
		while(shmem->flag[b] == waiting) {      // Exit CS
			b = (b + 1) % c;
		}
		printf(stderr, "\t\tChild PID: %i - Exiting Critical Section.\n\n", getpid());     // Print message that leaving CS

		shmem->turn = b;      // Give turn to next process 'wanting in CS'
		shmem->flag[i] = waiting;     		// Change flag to waiting
	}

	shmem->processes--;
	killAll();     // Kill all processes
	exit(3);     // Exit program
}

void signalHandler() {      // Kills all processes when signaled
    pid_t id = getpid();
	printf("Signal received - killing consumer with PID: %i.\n", id);
	killAll();
    killpg(id, SIGINT);
    exit(EXIT_SUCCESS);
}

void killAll() {      // Detaches shared memory
	shmdt(shmem);
}
