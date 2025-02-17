//#define _POSIX_C_SOURCE 200809L
#include <atomic.h>
#include <threads.h>

volatile int sequence_counter = 0;
thrd_mutex_t write_lock;

// Reader function
int reader(void) {
  int data_copy;

  // Reader performs a busy wait (can be replaced with backoff strategy)
  while (1) {
    int current_value = atomic_load(&sequence_counter);
    if (current_value % 2 == 0) { // Even value indicates stable data
      // Critical section for reading (protected by sequence counter)
      data_copy = 42; // Replace with actual data access
      // Process the data
      // ...
      break;
    }
  }

  // Reader successfully accessed consistent data
  return data_copy;
}

// Writer function
void writer(int new_value) {
  // Acquire the lock before modifying the data
  thrd_mutex_lock(&write_lock);

  // Critical section for writing (protected by external lock)
  sequence_counter++;  // Increment for write operation (odd value)
  // ... perform other write operations on the data ...

  // Release the lock after writing
  thrd_mutex_unlock(&write_lock);
}

int main() {
  // Initialize the mutex before creating threads
  thrd_mutex_init(&write_lock, NULL);

  // Create reader and writer threads (code not shown)

  // Wait for threads to finish (code not shown)

  // Destroy the mutex after use
  thrd_mutex_destroy(&write_lock);

  return 0;
}

