/* threads.h and stdatomic.h  w/ regard to C11 — C2x threads library
 * #Implementation#
 */

/* Thread Management Routines */
typedef struct thrd_t_struct thrd_t;
typedef int (*thrd_start_t)(void*);
int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
// thrd_equal - compares two thread identifiers for equality.
int thrd_equal(thrd_t lhs, thrd_t rhs);
/* thrd_current - returns the identifier (thread ID) of the calling thread.
   It's of type thrd_t, which may not have a fixed size or format across
   different implementations. Use %lu specifier */
thrd_t thrd_current(void);
// thrd_cleep - suspends the execution of the current thread for the specified duration.
int thrd_sleep(const struct timespec *duration, struct timespec *remaining);
// thrd_yield - yields the processor to another thread.
void thrd_yield(void);
// thrd_exit - exits the current thread with the specified result code.
[[noreturn]] void thrd_exit(int res);
// thrd - detaches a thread, allowing its resources to be reclaimed automatically when it exits.
int thrd_detach(thrd_t thr);
// thrd_join - waits for the specified thread to terminate and optionally retrieves its exit status.
int thrd_join(thrd_t thr, int *res);
enum {
    thrd_success = /* unspecified */,
    thrd_nomem = /* unspecified */,
    thrd_timedout = /* unspecified */,
    thrd_busy = /* unspecified */,
    thrd_error = /* unspecified */
};

/* 
 * TODO: in progress...
 *
 * TEMP: Attribute Object Routine (any amount of threads/cores)
 * For fine-grained control over thread creation and stack size
 * Utilize libraries such as unistd.h, sched.h and sys/wait.h
 *
 * This allows you to work w/ real operating system threads.
 *
 * The following co-routines are only for concurrent (single-threaded) systems.
 *
 * TEMP: Co-routines w/setjmp.h:
 * setjmp - which saves the current environment (stack pointer, program counter, etc.)
 * in a jmp_buf structure, and longjmp which restores it (allowing you to jump back to a specific point in the
 * code, effectively pausing and resuming execution)
 *
 * These allow you to simulate concurrency within a single thread by saving and restoring the execution state,
 * providing cooperative multitasking with manual control over context switching, involving less overhead but
 * lacking true parallelism.
 *
 * TEMP: Co-routines w/stdatomic.h:
 * In the context of concurrent operations on a single thread, atomic operations
 * would be considered suitable for managing shared variables and simple synchronization tasks. However, they're
 * not designed for handling I/O or complex event-driven programming. Instead they're best for operations that
 * can be expressed as atomic instructions (e.g., incrementing a counter, setting a flag). More complex
 * synchronization might still require additional mechanisms.
 *
 * TEMP: Co routines w/poll.h:
 * It may efficiently handle multiple I/O streams in a single thread, allowing the
 * application to wait for events on multiple file descriptors (Maybe suitable for network and other IO bound)
 * It does require a different programming model, perhaps considered more complex than using atomic libraries
 *
 * For a Thread-Local Memory Pool (abbr. T_MP) (Question: Allocate stacks manually w/ mmap too?)
 *
 * TEMP: Allocate a Chunk of Memory Per Thread (sys/mman.h  must be included for mmap)
 *
 * On thread creation, allocate a large block (say, 1MB) using mmap.
 * Store this block in a thread-local variable (__thread storage class in GCC/Clang, or thread_local in C11).
 *
 * Implement a "slab allocator" inside the block.
 * When the thread requests memory, allocate from its private block instead of calling malloc.
 * If a thread runs out of memory in its pool, request another block from mmap.
 * Free unused blocks when the thread exits.
 *
 * TEMP: Build a Lock-Free Allocator — To allocate memory directly from the OS
 * Uses thread-local pools to avoid contention.
 * Relies on atomic operations (like CAS) instead of mutexes (Note: CAS lets multiple threads allocate/free memory without locks)
 * Ensures low-latency allocations for high-performance multithreading.
 */

/* TEMP: Cleanup - in progress
 * cleanup_function - cleanup example in GCC for variables that fall out of scope */
void cleanup_function(void *p) {
    free(p); // Example cleanup action
}

// Function using __attribute__((cleanup))
void example_function() {
    // Allocate memory and associate cleanup function
    void *ptr __attribute__((cleanup(cleanup_function))) = malloc(10);
    // The cleanup_function will be called when ptr goes out of scope
    // or when example_function returns.
    // Use ptr...
}

/* Mutex Routines */
int mtx_init(mtx_t *mutex, int type);
int mtx_lock(mtx_t *mutex);
int mtx_timedlock(mtx_t *restrict mutex, const struct timespec *restrict time_point);
int mtx_trylock(mtx_t *mutex);
int mtx_unlock(mtx_t *mutex);
void mtx_destroy(mtx_t *mutex);
enum {
    mtx_plain = /* unspecified */,
    mtx_recursive = /* unspecified */,
    mtx_timed = /* unspecified */
};
void call_once(once_flag *flag, void (*func)(void));
typedef once_flag type-name
#define ONCE_FLAG_INIT definition

/* Condition Variable Routines */
int cnd_init(cnd_t *cond);
int cnd_wait(cnd_t *cond, mtx_t *mutex);
int cnd_signal(cnd_t *cond);
// cnd_broadcast - unblock all threads currently blocked on the specified condition variable.
int cnd_broadcast(cnd_t *cond);
int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mutex, const struct timespec *restrict time_point);
void cnd_destroy(cnd_t *cond);

/* Read/Write Lock Routines */
/* #Implementation# */
typedef struct {      // Data for a read/write lock
    mtx_t mtx;        // Mutex for controlling access to the lock
    cnd_t read_cond;  // Condition variable for waiting readers
    cnd_t write_cond; // Condition variable for waiting writers
    int num_readers;  // Number of readers currently holding the lock
    int num_writers;  // Number of writers currently holding the lock
    int num_pending_writers; // Number of pending writers
} rw_lock_t;

// TEMP: Make lock unusable, emulating destroy (pthread_rwlock_destroy)
void rw_lock_unusable(ReadWriteLock* lock) {
  pthread_mutex_lock(&lock->mtx);
  // Check if any threads are still using it
  if (lock->num_readers > 0 || lock->num_writers > 0 || lock->write_pending > 0) {
    pthread_mutex_unlock(&lock->mtx);
    // Error handling: Cannot destroy lock with active users
    return;
  }
  lock->destroyed = 1;
  pthread_cond_broadcast(&lock->write_cond); // Wake up any waiting writers (for cleanup)
  pthread_mutex_unlock(&lock->mtx);
}
// rw_lock_init - initialize the read/write lock
void rw_lock_init(rw_lock_t *lock) {
    mtx_init(&lock->mtx, mtx_plain);
    cnd_init(&lock->read_cond);
    cnd_init(&lock->write_cond);
    lock->num_readers = 0;
    lock->num_writers = 0;
    lock->num_pending_writers = 0;
}
// rw_lock_acquire - acquire the lock for reading, blocking until the lock can be acquired.
void rw_lock_acquire_read(rw_lock_t *lock) {
    mtx_lock(&lock->mtx);
    while (lock->num_writers > 0 || lock->num_pending_writers > 0) {
        cnd_wait(&lock->read_cond, &lock->mtx);
    }
    lock->num_readers++;
    mtx_unlock(&lock->mtx);
}
// rw_lock_try_acquire - attempt to acquire the lock for reading without blocking
bool rw_lock_try_acquire_read(rw_lock_t *lock) {
    mtx_lock(&lock->mtx);
    if (lock->num_writers > 0 || lock->num_pending_writers > 0) {
        mtx_unlock(&lock->mtx);
        return false;
    }
    lock->num_readers++;
    mtx_unlock(&lock->mtx);
    return true;
}
// rw_lock_acquire_write - acquire the lock for writing, blocking until the lock can be acquired.
void rw_lock_acquire_write(rw_lock_t *lock) {
    mtx_lock(&lock->mtx);
    lock->num_pending_writers++;
    while (lock->num_readers > 0 || lock->num_writers > 0) {
        cnd_wait(&lock->write_cond, &lock->mtx);
    }
    lock->num_pending_writers--;
    lock->num_writers++;
    mtx_unlock(&lock->mtx);
}
// rw_lock_try_acquire_write - attempt to acquire the lock for writing without blocking
bool rw_lock_try_acquire_write(rw_lock_t *lock) {
    mtx_lock(&lock->mtx);
    if (lock->num_readers > 0 || lock->num_writers > 0) {
        mtx_unlock(&lock->mtx);
        return false;
    }
    lock->num_pending_writers++;
    lock->num_writers++;
    mtx_unlock(&lock->mtx);
    return true;
}

// rw_lock_release_read - release/unlock the lock after reading
void rw_lock_release_read(rw_lock_t *lock) {
    mtx_lock(&lock->mtx);
    lock->num_readers--;
    if (lock->num_readers == 0 && lock->num_pending_writers > 0) {
        cnd_signal(&lock->write_cond);
    }
    mtx_unlock(&lock->mtx);
}
// rw_lock_release_write - release/unlock the lock after writing
void rw_lock_release_write(rw_lock_t *lock) {
    mtx_lock(&lock->mtx);
    lock->num_writers--;
    if (lock->num_pending_writers > 0) {
        cnd_signal(&lock->write_cond);
    } else {
        cnd_broadcast(&lock->read_cond);
    }
    mtx_unlock(&lock->mtx);
}

/* Read-Copy Update Routines */
/* #Implementation# */

/* Thread-Specific Storage Routines */
/* tss_dtor_t - defines a function pointer type tss_dtor_t for specifying destructor functions for thread-specific storage (TSS) keys.
   these destructor functions are called when a thread exits or when the TSS key associated with the thread is explicitly destroyed */
typedef void (*tss_dtor_t)(void *);
/* TSS_DTOR_ITERATIONS - defines the number of iterations used by the cleanup routine to call destructor functions for thread-specific storage keys.
   it specifies how many times the cleanup routine should attempt to call the destructor functions for each thread before giving up */
#define TSS_DTOR_ITERATIONS definition
// tss_create - creates a new thread-specific storage (TSS) key and associates it with the specified destructor function.
int tss_create(tss_t *tss_key, tss_dtor_t destructor);
/* tss_get - retrieves the value associated with the specified thread-specific storage (TSS) key for the current thread.
   it returns the value associated with the key or NULL if the key has no associated value for the current thread */
void *tss_get(tss_t tss_key);
// tss_set - associates the specified value with the specified thread-specific storage (TSS) key for the current thread.
int tss_set(tss_t tss_id, void *val);
/* tss_delete - destroys the specified thread-specific storage (TSS) key and disassociates it from any associated destructor function.
   it does not affect the values associated with the key in individual threads */
void tss_delete(tss_t tss_id);

/* Synchronization */
/* #Implementation# */
typedef struct {     // Semaphore structure
    mtx_t mutex;     // Mutex for mutual exclusion
    cnd_t condition; // Condition variable for thread blocking
    unsigned imp. value; // Semaphore value
} semaphore;
/* Initialize */
// sem_init - initializes a semaphore with a specified initial value.
int sem_init(semaphore *sem, unsigned int initial_value) {
    if (sem == NULL) return thrd_error;

    mtx_init(&sem->mutex, mtx_plain);
    cnd_init(&sem->condition);
    sem->value = initial_value;

    return thrd_success;
}
/* Wait
 *
 * sem_wait - decrements (waits on) a semaphore. If the semaphore's value is greater than zero, it decrements the value immediately;
 * otherwise, it blocks until the value becomes greater than zero.
 */
int sem_wait(semaphore *sem) {
    if (sem == NULL) return thrd_error;

    mtx_lock(&sem->mutex);
    while (sem->value == 0) {
        cnd_wait(&sem->condition, &sem->mutex);
    }
    sem->value--;
    mtx_unlock(&sem->mutex);

    return thrd_success;
}
/* Try Wait
 *
 * sem_trywait - attempts to decrement the semaphore value without blocking.
 * If the semaphore value is greater than zero, it decrements the value and returns immediately.
 * If the semaphore value is zero, it returns immediately without blocking.
 */
int sem_trywait(semaphore *sem) {
    if (sem == NULL) return thrd_error;

    mtx_lock(&sem->mutex);
    if (sem->value > 0) {
        sem->value--;
        mtx_unlock(&sem->mutex);
        return thrd_success;
    } else {
        mtx_unlock(&sem->mutex);
        return thrd_busy; // Semaphore value is zero, operation would block
    }
}
/* Signal */
// sem_signal - increments (signals) a semaphore, releasing a waiting thread if any.
int sem_signal(semaphore *sem) {
    if (sem == NULL) return thrd_error;

    mtx_lock(&sem->mutex);
    sem->value++;
    cnd_signal(&sem->condition);
    mtx_unlock(&sem->mutex);

    return thrd_success;
}
/* Destroy */
// sem_destroy - destroys a semaphore, releasing any associated resources.
void sem_destroy(semaphore *sem) {
    if (sem == NULL) return;

    mtx_destroy(&sem->mutex);
    cnd_destroy(&sem->condition);
}
/* Get Value
 *
 * sem_getvalue - retrieves the current value of the semaphore without modifying it.
 * It allows you to query the current state of the semaphore without blocking or modifying its value.
 */
int sem_getvalue(semaphore *sem, int *sval) {
    if (sem == NULL || sval == NULL) return thrd_error;

    mtx_lock(&sem->mutex);
    *sval = sem->value;
    mtx_unlock(&sem->mutex);

    return thrd_success;
}

/* Synchronization Point */
/* #Implementation# */
typedef struct {         // Barrier
    mtx_t mutex;         // Mutex for mutual exclusion
    cnd_t condition;     // Condition variable for thread blocking
    unsigned int count;  // Current count of arrived threads
    unsigned int total;  // Total number of threads expected to arrive at the barrier
} barrier;
/* Initialization */
// barrier_init - initializes a barrier with the specified @count of threads that must wait at le barrier.
int barrier_init(barrier *bar, unsigned int count) {
    if (bar == NULL || count == 0) return thrd_error;

    mtx_init(&bar->mutex, mtx_plain);
    cnd_init(&bar->condition);
    bar->count = 0;
    bar->total = count;

    return thrd_success;
}
/* Wait
 *
 * barrier_wait - increments the arrived threads count, checks if all threads have arrived,
 * and either waits until the required number of threads have called "" or releases
 */
int barrier_wait(barrier *bar) {
    if (bar == NULL) return thrd_error;

    mtx_lock(&bar->mutex);
    bar->count++;
    if (bar->count < bar->total) {
        // Not all threads have arrived, wait
        cnd_wait(&bar->condition, &bar->mutex);
    } else {
        // All threads have arrived, release
        bar->count = 0;
        cnd_broadcast(&bar->condition);
    }
    mtx_unlock(&bar->mutex);

    return thrd_success;
}
/* Destroy */
// barrier_destroy - destroys and releases said resources
void barrier_destroy(barrier *bar) {
    if (bar == NULL) return;

    mtx_destroy(&bar->mutex);
    cnd_destroy(&bar->condition);
}

/* TEMP: Cancellation - in progress */
/* #Implementation# */
static tss_t cancel_key;
static void cleanup_cancel_flag(void *value) {
    if (value != NULL) {
        free(value);
    }
}

static void initialize_cancellation_key() {
    tss_create(&cancellation_key, cleanup_cancel_flag);
}

static void set_cancel_flag() {
    int *value = malloc(sizeof(int));
    if (value != NULL) {
        *value = 1;
        tss_set(cancellation_key, value);
    }
}

static int get_cancel_flag() {
    int *value = tss_get(cancel_key);
    return (value != NULL) ? *value : 0;
}
int thrd_setcancelstate(int state, int *oldstate);
int thrd_setcanceltype(int type, int *oldtype) {
    // Not implemented yet
    // Store the cancel type in a global variable and provide a way to retrieve it
    return thrd_success;
}
/* thrd_testcancel - test for cancellation */
void thrd_testcancel(void) {
    // Check if cancellation has been requested
    if (atomic_load(&cancellation_requested)) {
        printf("Cancellation requested. Exiting...\n");
        thrd_exit(thrd_success);
    }
}

#include <stdatomic.h> /* atomic operations */
/* ATOMIC_FLAG_INIT - expands to an initializer that can be used to initialize atomic_flag type to the clear state.
 * if you declare an atomic_flag variable without explicitly initializing it using the ATOMIC_FLAG_INIT macro, its initial value is unspecified
 */
#define ATOMIC_FLAG_INIT

/* Atomic types have limited type conversion. atomic types can be associated w/ most data types including "least" and "fast", but not including "float"
 * type-name also cannot be atomic or cvr-qualified (const, volatile and reference pointers)
 */
typedef _Atomic _Bool                   atomic_bool;
typedef _Atomic char                    atomic_char;
typedef _Atomic signed char             atomic_schar;
typedef _Atomic unsigned char           atomic_uchar;
typedef _Atomic short                   atomic_short;
typedef _Atomic unsigned short          atomic_ushort;
typedef _Atomic int                     atomic_int;
typedef _Atomic unsigned int            atomic_uint;
typedef _Atomic long                    atomic_long;
typedef _Atomic unsigned long           atomic_ulong;
typedef _Atomic long long               atomic_llong;
typedef _Atomic unsigned long long      atomic_llong;
typedef _Atomic char8_t                 atomic_char8_t
typedef _Atomic char16_t                atomic_char16_t;
typedef _Atomic char32_t                atomic_char32_t;
typedef _Atomic wchar_t                 atomic_wchar_t;
typedef _Atomic int_least8_t            atomic_int_least8_t
typedef _Atomic uint_least8_t           atomic_uint_least8_t
typedef _Atomic int_least16_t           atomic_int_least16_t
typedef _Atomic uint_least16_t          atomic_uint_least16_t
typedef _Atomic int_least32_t           atomic_int_least32_t
typedef _Atomic uint_least32_t          atomic_uint_least32_t
typedef _Atomic int_least64_t           atomic_int_least64_t
typedef _Atomic uint_least64_t          atomic_uint_least64_t
typedef _Atomic int_fast8_t             atomic_int_fast8_t
typedef _Atomic uint_fast8_t            atomic_uint_fast8_t
typedef _Atomic int_fast16_t            atomic_int_fast16_t
typedef _Atomic uint_fast16_t           atomic_uint_fast16_t
typedef _Atomic int_fast32_t            atomic_int_fast32_t
typedef _Atomic uint_fast32_t           atomic_uint_fast32_t
typedef _Atomic int_fast64_t            atomic_int_fast64_t
typedef _Atomic uint_fast64_t           atomic_uint_fast64_t
typedef _Atomic intptr_t                atomic_intptr_t
typedef _Atomic uintptr_t               atomic_uintptr_t
typedef _Atomic size_t                  atomic_size_t
typedef _Atomic ptrdiff_t               atomic_ptrdiff_t
typedef _Atomic intmax_t                atomic_intmax_t
typedef _Atomic uintmax_t               atomic_uintmax_t

/* Use as a type qualifier; this designates the atomic version of type-name.
 * In this role, it may be mixed with const, volatile, and restrict, although unlike other qualifiers,
 * the atomic version of type-name may have a different size, alignment, and object representation */
typedef _Atomic(int) atomic_int;
/* Objects of atomic types are the only objects that are free from data races; that is, they may be modified by
 * two threads concurrently or modified by one and read by another.
 * Each atomic object has its own associated modification order, which is a total order of modifications made to
 * that object. if, from some thread's point of view, modification A of some atomic M happens-before modification B
 * of the same atomic M, then in the modification order of M, A occurs before B.
 * Note that although each atomic object has its own modification order, there is no single total order; different
 * threads may observe modifications to different atomic objects in different orders.
 * There are four coherences that are guaranteed for all atomic operations:
 * write-write, read-read, read-write and write-read coherence
 */ typedef _Atomic int atomic_int;

/* Initializes the default-constructed atomic object @obj w/ the value desired. It's important to note that this
 * initialization process itself is not atomic. This means that if there are multiple threads accessing  @obj
 * simultaneously, even if they're using atomic operations it could lead to a data race.
 * This is a generic function defined for all atomic object types A. The argument is pointer to a volatile atomic type
 * to accept addresses of both non-volatile and volatile (e.g. memory-mapped I/O) atomic objects, and volatile semantic
 * is preserved when applying this operation to volatile atomic objects. C is the non-atomic type corresponding to A.
 */
void atomic_init(volatile A *obj, C desired);

// NOTE: volatile types do not provide inter-thread synchronization, memory ordering, mutexes or atomicity!
typedef volatile type-name

#define ATOMIC_BOOL_LOCK_FREE
#define ATOMIC_CHAR_LOCK_FREE
#define ATOMIC_CHAR16_T_LOCK_FREE
#define ATOMIC_CHAR32_T_LOCK_FREE
#define ATOMIC_WCHAR_T_LOCK_FREE
#define ATOMIC_SHORT_LOCK_FREE
#define ATOMIC_INT_LOCK_FREE
#define ATOMIC_LONG_LOCK_FREE
#define ATOMIC_LLONG_LOCK_FREE
#define ATOMIC_POINTER_LOCK_FREE
#define ATOMIC_CHAR8_T_LOCK_FREE

/* memory_order - specifies how memory accesses, including regular, non-atomic memory accesses, are to be ordered around an atomic operation.
 *
 * Absent any constraints on a multi-core system, when multiple threads simultaneously read and write to several variables, one thread can
 * observe the values change in an order different from the order another thread wrote them. Indeed, the apparent order of changes can even
 * differ among multiple reader threads. Some similar effects can occur even on uniprocessor systems due to compiler transformations allowed
 * by the memory model.
 */
enum memory_order {
/* memory_order_relaxed - there are no synchronization or ordering constraints imposed on other reads or writes, only this operation's atomicity is guaranteed
 */ memory_order_relaxed,
/* memory_order_consume - a load operation with this memory order performs a consume operation on the affected memory location: no reads or writes in the current thread dependent
 * on the value currently loaded can be reordered before this load. Writes to data-dependent variables in other threads that release the same atomic variable
 * are visible in the current thread. On most platforms, this affects compiler optimizations only.
 */ memory_order_consume,
/* memory_order_acquire - a load operation with this memory order performs the acquire operation on the affected memory location: no reads or writes in the current thread can be re-
 * ordered before this load. All writes in other threads that release the same atomic variable are visible in the current thread.
 */ memory_order_acquire,
/* memory_order_release - a store operation with this memory order performs the release operation: no reads or writes in the current thread can be reordered after this store.
 * All writes in the current thread are visible in other threads that acquire the same atomic variable and writes that carry a dependency into the atomic
 * variable become visible in other threads that consume the same atomic.
 */ memory_order_release,
/* memory_order_acq_rel - a read-modify-write operation with this memory order is both an acquire operation and a release operation. No memory reads or writes in the current thread
 * can be reordered before the load, nor after the store. All writes in other threads that release the same atomic variable are visible before the modification
 * and the modification is visible in other threads that acquire the same atomic variable.
 */ memory_order_acq_rel,
/* memory_order_seq_cst - a load operation with this memory order performs an acquire operation, a store performs a release operation, and read-modify-write performs both an acquire
 * operation and a release operation, plus a single total order exists in which all threads observe all modifications in the same order.
 */ memory_order_seq_cst
};

/* kill_dependency - the function is implemented as a macro.
 * Informs the compiler that the dependency tree started by a memory_order_consume atomic load operation does not extend past the return value of kill_dependency;
 * that is, the argument does not carry a dependency into the return value */
A kill_dependency(A y);
/* atomic_thread_fence - establishes memory synchronization ordering of non-atomic and relaxed atomic accesses, as instructed by @order, without an associated atomic operation */
void atomic_thread_fence(memory_order order);
/* atomic_signal_fence - establishes memory synchronization ordering of non-atomic and relaxed atomic accesses, as instructed by @order, between a thread and a signal handler
 * executed on the same thread. This is equivalent to atomic_thread_fence, except no CPU instructions for memory ordering are issued.
 * Only reordering of the instructions by the compiler is suppressed as @order instructs */
void atomic_signal_fence(memory_order order);
/* atomic_is_lock_free - determines if the atomic operations on all objects of the type A (the type of the object pointed to by @obj) are lock-free.
 * In any given program execution, the result of calling atomic_is_lock_free is the same for all pointers of the same type.
 * This is a generic function defined for all atomic object types A. The argument is pointer to a volatile atomic type to
 * accept addresses of both non-volatile and volatile (e.g. memory-mapped I/O) atomic objects, and volatile semantic is
 * preserved when applying this operation to volatile atomic objects */
_Bool atomic_is_lock_free(const volatile A *obj);
/* atomic_store - atomically replaces the value of the atomic variable pointed to by @obj with @desired. The operation is atomic write operation.
 * The first version orders memory accesses according to memory_order_seq_cst, the second version orders memory accesses according to @order.
 * @order must be one of memory_order_relaxed, memory_order_release or memory_order_seq_cst. Otherwise the behavior is undefined.
 * This is a generic function defined for all atomic object types A. The argument is pointer to a volatile atomic type to accept
 * addresses of both non-volatile and volatile (e.g. memory-mapped I/O) atomic objects, and volatile semantic is preserved when
 * applying this operation to volatile atomic objects. C is the non-atomic type corresponding to A.
 */
void atomic_store(volatile A *obj , C desired);
/* atomic_store_explicit - "explicit" indicates that the memory ordering for the atomic operation is explicitly specified by the programmer */
void atomic_store_explicit(volatile A *obj, C desired, memory_order order);
/* atomic_load - atomically loads and returns the current value of the atomic variable pointed to by @obj. The operation is atomic read operation */
C atomic_load(const volatile A *obj);
C atomic_load_explicit(const volatile A *obj, memory_order order);
C atomic_exchange(volatile A *obj, C desired); // swaps a value with the value of an atomic object
C atomic_exchange_explicit(volatile A *obj, C desired, memory_order order);
// atomic_compare_exchange_strong - swaps a value with an atomic object if the old value is what is expected, otherwise reads the old value
_Bool atomic_compare_exchange_strong(volatile A *obj, C *expected, C desired);
// atomic_compare_exchange_weak - "strong" and "weak" refer to the level of guarantees provided by the atomic operations
_Bool atomic_compare_exchange_weak(volatile A *obj, C *expected, C desired);
_Bool atomic_compare_exchange_strong_explicit(volatile A *obj, C *expected, C desired, memory_order succ, memory_order fail);
_Bool atomic_compare_exchange_weak_explicit(volatile A *obj, C *expected, C desired, memory_order succ, memory_order fail);
// atomic_fetch_add - atomically replaces the value pointed by @obj with the result of addition of @arg to the old value of @obj, and returns the value @obj held previously.
// The operation is read-modify-write operation.
C atomic_fetch_add(volatile A *obj, M arg);
C atomic_fetch_add_explicit(volatile A *obj, M arg, memory_order order);
// atomic_fetch_sub - atomically replaces the value pointed by @obj with the result of subtraction of @arg from the old value of @obj, and returns the value @obj held previously.
// The operation is read-modify-write operation.
C atomic_fetch_sub( volatile A *obj, M arg );
C atomic_fetch_sub_explicit(volatile A *obj, M arg, memory_order order);

// atomic_flag - an atomic boolean type. Unlike other atomic types, it is guaranteed to be lock-free. Unlike atomic_bool, atomic_flag does not provide load or store operations.
struct atomic_flag;
// atomic_flag_test_and_set - atomically changes the state of a atomic_flag pointed to by @obj to set (true) and returns the previous value.
_Bool atomic_flag_test_and_set(volatile atomic_flag *obj);
_Bool atomic_flag_test_and_set_explicit(volatile atomic_flag *obj, memory_order order);
// atomic_flag_clear - atomically changes the state of a atomic_flag pointed to by @obj to clear (false).
void atomic_flag_clear(volatile atomic_flag *obj);
void atomic_flag_clear_explicit(volatile atomic_flag *obj, memory_order order);
