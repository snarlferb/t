//#define _POSIX_C_SOURCE 200809L
#include <threads.h>
#include <stdio.h>

typedef struct {
    mtx_t mutex;
    cnd_t readers_proceed;
    cnd_t writer_proceed;
    int pending_writers;
    int active_readers;
    int active_writers;
} rwlock_t;

void rwlock_init(rwlock_t *rwlock) {
    mtx_init(&rwlock->mutex, mtx_plain);
    cnd_init(&rwlock->readers_proceed);
    cnd_init(&rwlock->writer_proceed);
    rwlock->pending_writers = 0;
    rwlock->active_readers = 0;
    rwlock->active_writers = 0;
}

void rwlock_acquire_read(rwlock_t *rwlock) {
    mtx_lock(&rwlock->mutex);
    while (rwlock->pending_writers > 0 || rwlock->active_writers > 0) {
        cnd_wait(&rwlock->readers_proceed, &rwlock->mutex);
    }
    rwlock->active_readers++;
    mtx_unlock(&rwlock->mutex);
}

void rwlock_release_read(rwlock_t *rwlock) {
    mtx_lock(&rwlock->mutex);
    rwlock->active_readers--;
    if (rwlock->active_readers == 0 && rwlock->pending_writers > 0) {
        cnd_signal(&rwlock->writer_proceed);
    }
    mtx_unlock(&rwlock->mutex);
}

void rwlock_acquire_write(rwlock_t *rwlock) {
    mtx_lock(&rwlock->mutex);
    rwlock->pending_writers++;
    while (rwlock->active_readers > 0 || rwlock->active_writers > 0) {
        cnd_wait(&rwlock->writer_proceed, &rwlock->mutex);
    }
    rwlock->pending_writers--;
    rwlock->active_writers++;
    mtx_unlock(&rwlock->mutex);
}

void rwlock_release_write(rwlock_t *rwlock) {
    mtx_lock(&rwlock->mutex);
    rwlock->active_writers--;
    if (rwlock->pending_writers > 0) {
        cnd_signal(&rwlock->writer_proceed);
    } else {
        cnd_broadcast(&rwlock->readers_proceed);
    }
    mtx_unlock(&rwlock->mutex);
}

void rwlock_destroy(rwlock_t *rwlock) {
    mtx_destroy(&rwlock->mutex);
    cnd_destroy(&rwlock->readers_proceed);
    cnd_destroy(&rwlock->writer_proceed);
}

int main() {
    rwlock_t my_rwlock;
    rwlock_init(&my_rwlock);
    rwlock_destroy(&my_rwlock);
    return 0;
}

