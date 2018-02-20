OS Project 2 - Concurrent UNIX Processes and shared memory
Allie Robbins

The goal of this homework is to become familiar with concurrent processing in Unix/Linux using shared memory.

There are 2 programs for this project - master.c and consumer.c

Master.c - forks a producer and 20 child-consumers
         - has a timer for 100 seconds & ends program if 100 seconds pass
         - creates and attaches self to shared memory
         - write variables to shared memory
         - has functions to kill all processes and detach shared memory

Consumer.c - gets arguments from master.c
           - gets shared memory ID from master.c and attaches self to shared memory
           - implements algorithm for entering critical section
           - in CS - puts process to sleep for 0 - 5 seconds
           - writes to shared memory and exits
           - has functions to kill all processes and detach shared memory

I hate these damn projects. - Allie 
