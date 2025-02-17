//#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <threads.h>
#include <stdlib.h>
#include <stdatomic.h>

/* TODO: prio_chain */

// Define a task struct to represent a thread with its priority
typedef struct task {
  int tid;
  int priority;
  thrd_t thread;
  // ... other task specific data
} task_t;

// Mutex structure with a waiter list pointer (simplified, no RB-tree)
typedef struct mutex {
  _Atomic(int) locked; // Use atomic integer for lock state
  task_t* waiter_head;
} mutex_t;

// Function to initialize a mutex
void mutex_init(mutex_t* mtx) {
  mtx->locked = 0; // Initially unlocked (atomic write)
  mtx->waiter_head = NULL;
}

// Function to acquire a mutex (simplified, no priority inheritance)
void mutex_lock(mutex_t* mtx, task_t* task) {
  while (atomic_fetch_add_explicit(&mtx->locked, 1, memory_order_acq_rel) != 0) {
    // Add task to waiter list if already locked (simple example, not a PI chain)
    task_t* current = mtx->waiter_head;
    if (current == NULL) {
      mtx->waiter_head = task;
      task->next = NULL;
    } else {
      while (current->next != NULL) {
        current = current->next;
      }
      current->next = task;
    }
    // Spinlock until lock becomes available (replace with better waiting strategy)
  }
}

// Function to unlock a mutex
void mutex_unlock(mutex_t* mtx) {
  atomic_store_explicit(&mtx->locked, 0, memory_order_rel_acq); // Unlock atomically
  // Check if there are waiters and wake up the first one (no priority consideration)
  if (mtx->waiter_head != NULL) {
    task_t* waiter = mtx->waiter_head;
    mtx->waiter_head = waiter->next;
    // Simulate waking up the waiting task (replace with appropriate mechanism)
    printf("Waking up task %d\n", waiter->tid);
  }
}

// Example usage (replace task creation and management with your actual logic)
int main() {
  mutex_t my_mutex;
  mutex_init(&my_mutex);

  task_t task1 = {1, 10, 0}; // Create tasks with priorities
  task_t task2 = {2, 5, 0};

  // Spawn threads (replace with actual thread creation logic)
  thrd_create(&task1.thread, (void*)some_function, &task1);
  thrd_create(&task2.thread, (void*)some_function, &task2);

  // Simulate task1 acquiring the mutex first
  mutex_lock(&my_mutex, &task1);

  // Simulate task2 trying to acquire the mutex while task1 holds it
  mutex_lock(&my_mutex, &task2);

  // ... other program logic

  // Simulate task1 releasing the mutex
  mutex_unlock(&my_mutex);

  // Wait for threads to finish
  thrd_join(task1.thread, NULL);
  thrd_join(task2.thread, NULL);

  return 0;
}

// Replace this with your actual task logic
void* some_function(void* arg) {
  task_t* task = (task_t*)arg;
  printf("Task %d with priority %d running\n", task->tid, task->priority);
  // Simulate some work
  sleep(1);
  return NULL;
}

