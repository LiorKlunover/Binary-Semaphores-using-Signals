# Binary-Semaphores-using-Signals
This repository contains an example implementation of binary semaphores using POSIX signals and threads in C.
Binary Semaphores Using Signals
This repository contains an example implementation of binary semaphores using POSIX signals and threads in C. The program demonstrates how to use signals to synchronize two threads, ensuring mutual exclusion when accessing a shared global variable. This example provides insight into thread synchronization, signal handling, and critical sections in concurrent programming.

Files
main.c: The main source file containing the implementation of the semaphore-like mechanisms and thread functions.
Overview
The program performs the following tasks:

Initializes a signal set to handle SIGUSR1.
Sets up a signal handler for SIGUSR1.
Creates two threads that increment a shared global variable.
Uses sem_down and sem_up functions to synchronize the threads.
Implementation Details
Global Variables:

sigset_t signal_set: A signal set containing SIGUSR1.
int signal_flag: A flag set by the signal handler to indicate the reception of a signal.
pthread_t thread1, thread2: Thread identifiers for the two threads.
int global: A shared global variable representing the critical section.
Signal Handler:

The signal_handler function sets the signal_flag to 1 upon receiving SIGUSR1.
Semaphore-like Functions:

sem_down(): Blocks the calling thread until signal_flag is set, indicating that the thread can enter the critical section.
sem_up(pthread_t thread): Sends SIGUSR1 to the specified thread, allowing it to proceed.
Thread Functions:

Each thread repeatedly calls sem_down to wait for its turn, enters the critical section to update the shared global variable, and then calls sem_up to signal the other thread.
Main Function:

Initializes the signal set and registers the signal handler.
Creates two threads.
Starts the first thread by sending SIGUSR1.
Waits for both threads to finish execution.
