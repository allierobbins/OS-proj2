OS Project 2 - Concurrent UNIX Processes and shared memory
Allie Robbins

The goal of this homework is to become familiar with concurrent processing in Unix/Linux using shared memory.

There are 5 programs/files for this project - master.c, consumer.c, producer.c, inputDataFile.txt, and Makefile

Master.c - forks a producer and 20 child-consumers
         - has a timer for 100 seconds & ends program if 100 seconds pass
         - creates and attaches self to shared memory
         - write variables to shared memory
         - has functions to check PID status, end processes, and detach shared memory

Consumer.c - gets arguments from master.c
           - gets shared memory ID from master.c and attaches self to shared memory
           - implements algorithm for entering critical section
           - in CS - puts process to sleep for 0 - 5 seconds
           - writes to shared memory and exits
           - has functions to end processes and detach shared memory

producer.c - gets arguments from master.c
           - gets shared memory ID from master.c and attaches self to shared memory
           - checks on status of PIDs from consumer.c
           - decides whether master.c continues or waits
           - has functions to end processes and detach shared memory

inputDataFile.txt - text file with the first 50 lines from the Bee Movie
                  - saw that this was needed at the last minute
                  - don't know what it's for?

Makefile - contains compile commands for master.c, consumer.c, and producer.c
         - contains commands to clean up created files when done

I hate these damn projects. - Allie

