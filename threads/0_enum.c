//#define _POSIX_C_SOURCE 200809L
// Lock class types
typedef enum {
    LOCK_CLASS_MUTEX,
    LOCK_CLASS_FILE,
    LOCK_CLASS_PROCESS,
    LOCK_CLASS_RESOURCE
} lock_class_t;

typedef struct {           // Lock structure
    lock_class_t class;
    union {
        mtx_t mutex;       // For mutex locks
        int fd;            // File descriptor for file locks
        pid_t pid;         // Process ID for process synchronization
        // Add other lock types...
    } lock;
} lock_t;

