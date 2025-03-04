#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

// Task structure
typedef struct task {
    void (*function)(void *arg);
    void *arg;
    struct task *next;
} task_t;

// Workqueue structure
typedef struct {
    mtx_t mutex;
    cnd_t condition;
    task_t *head;
    task_t *tail;
    int stop;
} workqueue_t;

// Worker thread function
int worker_thread(void *arg) {
    workqueue_t *queue = (workqueue_t *)arg;

    while (1) {
        mtx_lock(&queue->mutex);

        while (queue->head == NULL && !queue->stop) {
            cnd_wait(&queue->condition, &queue->mutex);
        }

        if (queue->stop) {
            mtx_unlock(&queue->mutex);
            break;
        }

        task_t *task = queue->head;
        queue->head = task->next;
        if (queue->head == NULL) {
            queue->tail = NULL;
        }

        mtx_unlock(&queue->mutex);

        task->function(task->arg);
        free(task);
    }

    return 0;
}

// Initialize the workqueue
int workqueue_init(workqueue_t *queue, int num_threads) {
    if (queue == NULL) return thrd_error;

    mtx_init(&queue->mutex, mtx_plain);
    cnd_init(&queue->condition);
    queue->head = NULL;
    queue->tail = NULL;
    queue->stop = 0;

    for (int i = 0; i < num_threads; ++i) {
        thrd_t thread;
        if (thrd_create(&thread, worker_thread, queue) != thrd_success) {
            return thrd_error;
        }
        thrd_detach(thread);
    }

    return thrd_success;
}

// Add a task to the workqueue
int workqueue_add_task(workqueue_t *queue, void (*function)(void *), void *arg) {
    if (queue == NULL || function == NULL) return thrd_error;

    task_t *task = malloc(sizeof(task_t));
    if (task == NULL) return thrd_error;

    task->function = function;
    task->arg = arg;
    task->next = NULL;

    mtx_lock(&queue->mutex);

    if (queue->tail == NULL) {
        queue->head = task;
        queue->tail = task;
    } else {
        queue->tail->next = task;
        queue->tail = task;
    }

    cnd_signal(&queue->condition);
    mtx_unlock(&queue->mutex);

    return thrd_success;
}

// Destroy the workqueue
void workqueue_destroy(workqueue_t *queue) {
    if (queue == NULL) return;

    mtx_lock(&queue->mutex);
    queue->stop = 1;
    cnd_broadcast(&queue->condition);
    mtx_unlock(&queue->mutex);

    // Allow worker threads to exit and clean up resources
    mtx_destroy(&queue->mutex);
    cnd_destroy(&queue->condition);
}

// Example task function
void example_task(void *arg) {
    int *num = (int *)arg;
    printf("Task: %d\n", *num);
}

int main() {
    workqueue_t queue;
    workqueue_init(&queue, 4);

    for (int i = 0; i < 10; ++i) {
        int *num = malloc(sizeof(int));
        *num = i;
        workqueue_add_task(&queue, example_task, num);
    }

    // Give some time for tasks to be processed
    thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);
    workqueue_destroy(&queue);
    return 0;
}

