#include <pthread.h>
#include <signal.h>
#include <stdio.h>

#include <unistd.h>

// Global variables
sigset_t signal_set;
int signal_flag = 0;
pthread_t thread1, thread2;
int global =0; // Critical section

// Signal handler
void signal_handler(int signum) {
    signal_flag = 1;
}

// Semaphore-like functions
void sem_down() {
    sigset_t old_set;
    sigprocmask(SIG_BLOCK, &signal_set, &old_set);

    while (!signal_flag) {
        sigsuspend(&old_set);
    }
    signal_flag = 0;

    sigprocmask(SIG_SETMASK, &old_set, NULL);
}

void sem_up(pthread_t thread) {
    pthread_kill(thread, SIGUSR1);
}

// Thread functions
void* thread_function(void* arg) {
    pthread_t other_thread = (pthread_self() == thread1) ? thread2 : thread1;

    for (int i = 0; i < 5; ++i) {
        printf("Thread %ld is waiting...\n", (long)arg);
        sem_down();

        // Block signals while in critical section
        sigset_t old_set;
        pthread_sigmask(SIG_BLOCK, &signal_set, &old_set);

        printf("Thread %ld is running...\n", (long)arg);
        global++;
        printf("Global: %d\n", global);
        // Unblock signals after critical section
        pthread_sigmask(SIG_SETMASK, &old_set, NULL);

        sem_up(other_thread);
    }
    return NULL;
}

int main() {
    // Initialize the signal set and add SIGUSR1 to it
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGUSR1);

    // Register the signal handler
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    // Create the threads
    if (pthread_create(&thread1, NULL, thread_function, (void*)1L) != 0) {
        perror("pthread_create");
        return 1;
    }

    if (pthread_create(&thread2, NULL, thread_function, (void*)2L) != 0) {
        perror("pthread_create");
        return 1;
    }

    // Let the threads start running
    sleep(1);
    sem_up(thread1);  // Unblock the first thread

    // Wait for the threads to finish
    if (pthread_join(thread1, NULL) != 0) {
        perror("pthread_join");
        return 2;
    }

    if (pthread_join(thread2, NULL) != 0) {
        perror("pthread_join");
        return 2;
    }

    printf("Threads have finished execution.\n");
    return 0;
}