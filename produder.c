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
	int maxWrites;
	int consumerProcesses;
	int processes;
};

enum state {waiting, wantInCS, inCriticalSection};      // States for CS flags

int shmID;      //  Global variables
struct SharedMemory *shmem;

int main(int argc, char* argv[]) {

	signal(SIGINT, endProgram);     // Signal Handler

	if((shmID = shmget(key, sizeof(struct SharedMemory) * 2, 0666)) < 0) {     // Get shared memory id from master process                                                         // If no shared memory id is caught - exit
		perror("Error when getting shared memory ID.\n");
		exit(1);
	}

    if ((shmem = (struct SharedMemory *)shmat(shmID, NULL, 0)) == (struct SharedMemory *) -1) {     	// Attach shared memory to consumer process
		perror("Error when attaching to shared memory.\n");      // Error message if shared memory is not attached & exit
        exit(1);
    }

      //printf("My PID is %ld.\n", (long)getpid());

      int i, status;     //variables

    	for(i = 0; i < size; i++) {      // Check PIDs status
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


    return 0;
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
