//#define _POSIX_C_SOURCE 200809L
#include <threads.h>
#include <stdio.h>

// Example function using a mutex lock
void critical_section_function(lock_t *lock) {
    if (lock->class != LOCK_CLASS_MUTEX) {
        fprintf(stderr, "Invalid lock class for this function\n");
        return;
    }

    mtx_lock(&lock->lock.mutex);
    // Critical section code here
    mtx_unlock(&lock->lock.mutex);
}

int main() {
    lock_t my_lock;
    my_lock.class = LOCK_CLASS_MUTEX;
    mtx_init(&my_lock.lock.mutex, mtx_plain);

    critical_section_function(&my_lock);

    mtx_destroy(&my_lock.lock.mutex);
    return 0;
}

