//#define _POSIX_C_SOURCE 200809L
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

// Example function using process synchronization
void process_sync_function(lock_t *lock) {
    if (lock->class != LOCK_CLASS_PROCESS) {
        fprintf(stderr, "Invalid lock class for this function\n");
        return;
    }

    pid_t child_pid = fork();
    if (child_pid == 0) {
        // Child process
        printf("Child process\n");
        _exit(0);
    } else {
        // Parent process
        waitpid(lock->lock.pid, NULL, 0);
        printf("Child process has terminated\n");
    }
}

int main() {
    lock_t my_lock;
    my_lock.class = LOCK_CLASS_PROCESS;
    my_lock.lock.pid = getpid();

    process_sync_function(&my_lock);

    return 0;
}

