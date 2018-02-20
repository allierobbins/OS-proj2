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

void endProgram();     // Prototypes
int detachAndRemove(int, void *);

struct SharedSpace {     // Struct for shared memory
	int sharedVar;
	int turn;
	int flag[];
	int consumerProcesses;
	int processes;
};

enum state {waiting, wantInCS, inCriticalSection};      // States for CS flags

int shmID;      //  Global variables
struct SharedSpace *SM;

int main(int argc, char* argv[]) {

	signal(SIGINT, endProgram);     // Signal Handler

	if((shmID = shmget(key, sizeof(struct SharedMemory) * 2, 0666)) < 0) {     // Get shared memory id from master process                                                         // If no shared memory id is caught - exit
		perror("Error when getting shared memory ID.\n");
		exit(1);
	}

    if ((SM = (struct SharedMemory *)shmat(shmID, NULL, 0)) == (struct SharedMemory *) -1) {     	// Attach shared memory to consumer process
		perror("Error when attaching to shared memory.\n");      // Error message if shared memory is not attached & exit
        exit(1);
    }


    //printf("My PID is %ld.\n", (long)getpid());

	int a;
  int b, c = consumerProcesses;      // Algorithm for CS entry

	do {
		do {

			SM.flag[i] = wantInCS;     // Set flags to 'wanting in CS'
			b = *turn;     // Assignment
			while(b != i)
				b = (SM.flag[b] != waiting) ? *turn : (b + 1) % c;      // Set flag to 'waiting'

			SM.flag[i] = inCriticalSection;      // Declare wanting to enter CS

			for(b = 0; b < c; b++)
				if((b != i) && (SM.flag[b] == inCriticalSection))     // See if anyone else is in CS
					break;

		} while((b < c) || ((*turn != i) && SM.flag[*turn] != waiting));

		*turn = i;      // Assign to self - go into CS
		printf("pid %i - entering Critical Section.\n", getpid());      //CS

		srand((unsigned int)time(NULL));
		sleep((rand() % 5));        		// Sleep for 0 - 5 seconds

		sharedVar++;     		// Increase sharedVar in shared memory

		printf("File modified by process number %i at time %s with sharedNum = %d.\n", i, t, SM.sharedVar);      // Print log message

		sleep((rand() % 5));      	// Sleep for 0 - 5 seconds


		b = (*turn + 1) % c;
		while (flag[b] == waiting) {      // Exit CS
			b = (b + 1) % c;
		}
		printf("Child PID: %i - Leaving Critical Section.\n", (long)getpid());     // Print message that leaving CS

		*turn = b;      // Give turn to next process 'wanting in CS'
		SM.flag[i] = waiting;     		// Change flag to waiting
	} while (1);

	SM.processes--;

  if (detachAndRemove(id, sharedtotal) == -1) {     // Detach shared memory
  perror("Failed to destroy shared memory segment");      //
  return 1;
}

	exit(1);     // Exit program
}

void endProgram() {      // Kills all processes when signaled

  pid_t id = getpid();

  if (detachAndRemove(id, sharedtotal) == -1) {     // Detach shared memory
  perror("Failed to destroy shared memory segment");      //
  return 1;
  }

    killpg(id, SIGINT);
    exit(EXIT);
}

int detachAndRemove(int shmID, void *shmaddr) {
  int error = 0;

  if (shmdt(shmaddr) == -1)
    error = errno;
  if ((shmctl(shmID, IPC_RMID, NULL) == -1) && !error)
    error = errno;
  if (!error)
    return 0;
    errno = error;
    return -1;
}
