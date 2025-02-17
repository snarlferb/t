#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

// Event callback function type
typedef void (*event_callback_t)(void *arg);

// Event bus structure
typedef struct {
    mtx_t mutex;
    event_callback_t *callbacks;
    void **callback_args;
    int callback_count;
    int capacity;
} event_bus_t;

// Initialize the event bus
int event_bus_init(event_bus_t *bus, int capacity) {
    if (bus == NULL) return thrd_error;

    mtx_init(&bus->mutex, mtx_plain);
    bus->callbacks = malloc(capacity * sizeof(event_callback_t));
    bus->callback_args = malloc(capacity * sizeof(void *));
    if (bus->callbacks == NULL || bus->callback_args == NULL) return thrd_error;
    
    bus->callback_count = 0;
    bus->capacity = capacity;

    return thrd_success;
}

// Register an event callback
int event_bus_register(event_bus_t *bus, event_callback_t callback, void *arg) {
    if (bus == NULL || callback == NULL) return thrd_error;

    mtx_lock(&bus->mutex);

    if (bus->callback_count >= bus->capacity) {
        mtx_unlock(&bus->mutex);
        return thrd_error;
    }

    bus->callbacks[bus->callback_count] = callback;
    bus->callback_args[bus->callback_count] = arg;
    bus->callback_count++;

    mtx_unlock(&bus->mutex);

    return thrd_success;
}

// Trigger an event
void event_bus_trigger(event_bus_t *bus) {
    if (bus == NULL) return;

    mtx_lock(&bus->mutex);

    for (int i = 0; i < bus->callback_count; ++i) {
        bus->callbacks[i](bus->callback_args[i]);
    }

    mtx_unlock(&bus->mutex);
}

// Destroy the event bus
void event_bus_destroy(event_bus_t *bus) {
    if (bus == NULL) return;

    mtx_destroy(&bus->mutex);
    free(bus->callbacks);
    free(bus->callback_args);
}

// Example callback function
void example_callback(void *arg) {
    int *num = (int *)arg;
    printf("Event: %d\n", *num);
}

int main() {
    event_bus_t bus;
    event_bus_init(&bus, 10);

    for (int i = 0; i < 5; ++i) {
        int *num = malloc(sizeof(int));
        *num = i;
        event_bus_register(&bus, example_callback, num);
    }

    // Trigger the event
    event_bus_trigger(&bus);

    event_bus_destroy(&bus);
    return 0;
}

