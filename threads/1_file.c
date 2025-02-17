//#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

// Example function using a file lock
void file_lock_function(lock_t *lock) {
    if (lock->class != LOCK_CLASS_FILE) {
        fprintf(stderr, "Invalid lock class for this function\n");
        return;
    }

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0; // Lock the whole file

    fcntl(lock->lock.fd, F_SETLKW, &fl);
    // File access code here
    fl.l_type = F_UNLCK;
    fcntl(lock->lock.fd, F_SETLK, &fl);
}

int main() {
    lock_t my_lock;
    my_lock.class = LOCK_CLASS_FILE;
    my_lock.lock.fd = open("example.txt", O_RDWR);

    if (my_lock.lock.fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    file_lock_function(&my_lock);

    close(my_lock.lock.fd);
    return 0;
}

