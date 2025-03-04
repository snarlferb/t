//#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS 4
#define ITERATIONS 10000

// Function to perform a computationally intensive task
void compute_task(int thread_id) {
    for (int i = 0; i < ITERATIONS; ++i) {
        // Simulate a computationally intensive operation
        double result = 0.0;
        for (int j = 0; j < 1000000; ++j) {
            result += sin(j) * cos(j);
        }
    }
    printf("Thread %d completed its task.\n", thread_id);
}

int main() {
    // Array to hold thread objects
    thrd_t threads[NUM_THREADS];

    // Start the timer
    clock_t start_time = clock();

    // Create and launch threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (thrd_create(&threads[i], (thrd_start_t)compute_task, (void*)(intptr_t)i) != thrd_success) {
            fprintf(stderr, "Error creating thread %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (thrd_join(threads[i], NULL) != thrd_success) {
            fprintf(stderr, "Error joining thread %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // End the timer
    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Total execution time: %.2f seconds.\n", duration);

    return 0;
}
