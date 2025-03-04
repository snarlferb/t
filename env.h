/* env.h
 * #Implementation#
 * Note: Please see the manpage for a complete list
 */
#include <stdlib.h>
/* Every complete object type has a property called alignment requirement, which is an integer value of type size_t representing the number of bytes between successive addresses
 * at which objects of this type can be allocated. The valid alignment values are non-negative integral powers of two. Since C11 -> the alignment requirement of a type can be
 * queried with _Alignof. In order to satisfy alignment requirements of all members of a struct, padding may be inserted after some of its members.
 *
 * malloc - allocates @size bytes of uninitialized storage. If allocation succeeds, returns a pointer that is suitably aligned for any object type with fundamental alignment.
 * If @size is zero, the behavior of malloc is implementation-defined. For example, a null pointer may be returned. Alternatively, a non-null pointer may be returned;
 * but such a pointer should not be dereferenced, and should be passed to free to avoid memory leaks */
void *malloc(size_t size);
/* calloc - allocates memory for an array of @num objects of @size and initializes all bytes in the allocated storage to zero. If allocation succeeds, returns a pointer to the
 * lowest (first) byte in the allocated memory block that is suitably aligned for any object type with fundamental alignment.
 * If size is zero, the behavior is implementation defined (null pointer may be returned, or some non-null pointer may be returned that may not be used to access storage) */
void *calloc(size_t num, size_t size);
/* realloc - reallocates the given area of memory. If ptr is not NULL, it must be previously allocated by malloc, calloc or realloc and not yet freed with a call to free or realloc.
 * Otherwise, the results are undefined. The reallocation is done by either: a) expanding or contracting the existing area pointed to by @ptr, if possible.
 * The contents of the area remain unchanged up to the lesser of the new and old sizes. If the area is expanded, contents of the new part of the array are undefined.
 * b) allocating a new memory block of size @new_size bytes, copying memory area with size equal the lesser of the new the old sizes, and freeing the old block.
 * If there is not enough memory, the old memory block is not freed and null pointer is returned. If @ptr is NULL, the behavior is the same as calling malloc(@new_size). (see documentation for more) */
void *realloc(void *ptr, size_t new_size);	
/* free - deallocates the space previously allocated by malloc(), calloc(), aligned_alloc(),(since C11) or realloc(). If ptr is a null pointer, the function does nothing.
 * The behavior is undefined if the value of ptr does not equal a value returned earlier by malloc(), calloc(), realloc(), or aligned_alloc()(since C11). see documentation on more undefined behaviors */
void free(void *ptr);
/* free_sized - deallocates the space previously allocated by malloc(), calloc(), or realloc() (but not aligned_alloc()) */
void free_sized(void *ptr, size_t size);
/* free_aligned_sized - if @ptr is a null pointer or the result obtained from a call to aligned_alloc, where alignment is equal to the requested allocation alignment and size is equal to the
 * requested allocation size, this function is equivalent to free(ptr). Otherwise, the behavior is undefined */
void free_aligned_sized(void *ptr, size_t alignment, size_t size);
/* aligned_alloc - allocate @size bytes of uninitialized storage whose alignment is specified by alignment. The @size parameter must be an integral multiple of alignment.
 * aligned_alloc is thread-safe: it behaves as though only accessing the memory locations visible through its argument, and not any static storage.
 * A previous call to free or realloc that deallocates a region of memory synchronizes-with a call to aligned_alloc that allocates the same or a part of the same region of memory.
 * This synchronization occurs after any access to the memory by the deallocating function and before any access to the memory by aligned_alloc.
 * There is a single total order of all allocation and deallocation functions operating on each particular region of memory */
void *aligned_alloc(size_t alignment, size_t size);
char *mkdtemp(char *template); // creates a directory with a unique name derived from template.
int mkstemp(char *template);
/* system - calls the host environment's command processor with the parameter @command. Returns an implementation-defined value (usually the value that the invoked program returns).
 * If @command is a null pointer, checks if the host environment has a command processor and returns a nonzero value if and only if the command processor exists */
int system(const char *command);
/* getenv - searches for an environmental variable with name @name in the host-specified environment list and returns a pointer to the string that is associated with the
 * matched environment variable. The set of environmental variables and methods of altering it are implementation-defined (see elsewhere for more complete definition) */
char *getenv(const char *name);
errno_t getenv_s(size_t *restrict len, char *restrict value, rsize_t valuesz, const char *restrict name); // (see elsewhere for a complete definition).
/* abort - (implementation since C23) Causes abnormal program termination unless SIGABRT is being caught by a signal handler passed to signal and the handler does not return.
// Functions passed to atexit() are not called. Whether open resources such as files are closed is implementation defined.
// (Implementation since C23) An implementation defined status is returned to the host environment that indicates unsuccessful execution */
[[noreturn]] void abort(void);
/* exit - causes normal program termination to occur. Several cleanup steps are performed:
 * functions passed to atexit are called, in reverse order of registration, all C streams are flushed and closed
 * files created by tmpfile are removed, control is returned to the host environment. If @exit_code is zero or EXIT_SUCCESS,
 * an implementation-defined status indicating successful termination is returned. If @exit_code is EXIT_FAILURE, an implementation
 * -defined status indicating unsuccessful termination is returned. In other cases implementation-defined status value is returned */
[[noreturn]] void exit(int exit_code);
/* quick_exit - causes normal program termination to occur without completely cleaning the resources.
 * Functions passed to at_quick_exit are called in reverse order of their registration. After calling the registered functions, calls _Exit(exit_code).
 * Functions passed to atexit or signal handlers passed to  signal  are not called */
[[noreturn]] void quick_exit(int exit_code);
/* _Exit - causes normal program termination to occur without completely cleaning the resources.
 * Functions passed to at_quick_exit() or atexit() are not called. Whether open streams with unwritten buffered data are flushed,
 * open streams are closed, or temporary files are removed is implementation-defined */
[[noreturn]] void _Exit(int exit_code);
/* atexit - registers the function pointed to by func to be called on normal program termination (via exit() or returning from main()).
 * The functions will be called in reverse order they were registered, i.e. the function registered last will be executed first.
 * The same function may be registered more than once. atexit is thread-safe: calling the function from several threads does not induce a data race.
 * The implementation is guaranteed to support the registration of at least 32 functions. The exact limit is implementation-defined */
int atexit(void (*func)(void));
/* at_quick_exit - registers the function pointed to by func to be called on quick program termination (via quick_exit).
 * Calling the function from several threads does not induce a data race. The implementation is guaranteed to support the
 * registration of at least 32 functions. The exact limit is implementation-defined.
 * The registered functions will not be called on normal program termination (exit)
 * If a function need to be called in that case, atexit must be used */
int at_quick_exit(void (*func)(void));
#define EXIT_SUCCESS
#define EXIT_FAILURE
void abort_handler_s(const char *restrict msg, void *restrict ptr, errno_t error);
void ignore_handler_s(const char *restrict msg, void *restrict ptr, errno_t error);
/* constraint_handler_t - on a runtime constraint violation, it is called with the following arguments:
 * 1) pointer to character string that describes the error
 * 2) pointer to an implementation-defined object or a null pointer. Examples of implementation-defined objects are objects that give the
 *    name of the function that detected the violation and the line number when the violation was detected
 * 3) the @error about to be returned by the calling function, if it happens to be one of the functions that return errno_t */
typedef void (*constraint_handler_t)(const char *restrict msg, void *restrict ptr, errno_t error);
// set_constraint_handler_s - configures the handler to be called by all bounds-checked functions on a runtime constraint violation
// or restores the default handler (if @handler is a null pointer)
// The @handler must be a pointer to function of type constraint_handler_t */
constraint_handler_t set_constraint_handler_s(constraint_handler_t handler); 

int abs(int n);
long labs(long n);
long long llabs(long long n);
div_t div(int x, int y);
ldiv_t ldiv(long x, long y);
lldiv_t lldiv(long long x, long long y);
/* mblen - determines the size, in bytes, of the multibyte character whose first byte is pointed to by  @s
 * If @s is a null pointer, resets the global conversion state and (until C23) determined whether shift sequences are used */
int mblen(const char *s, size_t n);
/* mbtowc - converts a multibyte character whose first byte is pointed to by  @s  to a wide character, written to @pwc if pwc is not null */
int mbtowc(wchar_t *restrict pwc, const char *restrict s, size_t n);
/* wctomb - converts a wide character @wc to multibyte encoding and stores it (including any shift sequences) in the char array whose first element is pointed to by @s
 * No more than MB_CUR_MAX characters are stored. The conversion is affected by the current locale's LC_CTYPE category */
int wctomb(char *s, wchar_t wc); 
errno_t wctomb_s(int *restrict status, char *restrict s, rsize_t ssz, wchar_t wc);
/* mbstowcs - converts a multibyte character string from the array whose first element is pointed to by src to its wide character representation
 * Converted characters are stored in the successive elements of the array pointed to by dst. No more than @len wide characters are written to the destination array */
size_t mbstowcs(wchar_t *restrict dst, const char *restrict src, size_t len)
errno_t mbstowcs_s(size_t *restrict retval, wchar_t *restrict dst, rsize_t dstsz, const char *restrict src, rsize_t len);
/* wcstombs - converts a sequence of wide characters from the array whose first element is pointed to by @src to its narrow multibyte representation that begins in the initial shift state.
 * Converted characters are stored in the successive elements of the char array pointed to by @dst. No more than @len bytes are written to the destination array */
size_t wcstombs(char *restrict dst, const wchar_t *restrict src, size_t len);
errno_t wcstombs_s(size_t *restrict retval, char *restrict dst, rsize_t dstsz, const wchar_t *restrict src, rsize_t len);
#define MB_CUR_MAX // maximum number of bytes in a multibyte character, in the current locale
/* qsort - sorts the given array pointed to by @ptr in ascending order. The array contains @count elements of size bytes. Function pointed to by @comp is used for object comparison */
void qsort(void *ptr, size_t count, size_t size, int (*comp)(const void*, const void*));
errno_t qsort_s(void *ptr, rsize_t count, rsize_t size, int (*comp)(const void*, const void*, void*), void *context);
/* bsearch - finds an element equal to element pointed to by @key in an array pointed to by @ptr. The array contains @count elements of size bytes and must be partitioned with respect to @key,
 * that is, all the elements that compare less than must appear before all the elements that compare equal to, and those must appear before all the elements that compare greater than the key object.
 * A fully sorted array satisfies these requirements. The elements are compared using function pointed to by @comp. The behavior is undefined if the array is not already partitioned with respect to
 * @key in ascending order according to the same criterion that @comp uses */
void *bsearch(const void *key, const void *ptr, size_t count, size_t size, int (*comp)(const void*, const void*));
void *bsearch_s(const void *key, const void *ptr, rsize_t count, rsize_t size, int (*comp)(const void *, const void *, void *), void *context);
/*QVoid*/ *bsearch(const void *key, /*QVoid*/ *ptr, size_t count, size_t size, int (*comp)(const void*, const void*));
/*QVoid*/ *bsearch_s(const void *key, /*QVoid*/ *ptr, rsize_t count, rsize_t size, int (*comp)(const void *, const void *, void *), void *context);

#include <signal.h>
/* signal - sets the error handler for signal @sig. The signal @handler can be set so that default handling will occur, signal is ignored, or a user-defined function is called.
 * When signal @handler is set to a function and a signal occurs, it is implementation defined whether signal(sig, SIG_DFL) will be executed immediately before the start of signal @handler.
 * Also, the implementation can prevent some implementation-defined set of signals from occurring while the signal @handler runs */
void (*signal(int sig, void (*handler) (int))) (int);
int raise(int sig); // Sends signal @sig to the program. The signal @handler, specified using signal(), is invoked.
typedef sig_atomic_t; // An integer type which can be accessed as an atomic entity even in the presence of asynchronous interrupts made by signals.
#define SIG_DFL // Expand into integral expressions that are not equal to an address of any function. The macros define signal handling strategies for signal() function.
#define SIG_IGN
#define SIG_ERR // A value of type void(*)(int). When returned by signal, indicates that an error has occurred.
#define SIGTERM // Termination signal
#define SIGSEGV // void *si_addr  or  Address of faulting memory reference.    
#define SIGINT // Termination interrupt signal
#define SIGILL // void *si_addr   or  Address of faulting instruction.
#define SIGABRT // Process abort signal.
#define SIGFPE // Erroneous arithmetic operation.

#define SIG_DFL // Request for default signal handling.
#define SIG_HOLD // Request that signal be held.
#define SIG_IGN // Request that signal be ignored.
#define SIGEV_NONE // No asynchronous notification is delivered when the event of interest occurs.
#define SIGEV_SIGNAL // A queued signal, with an application-defined value, is generated when the event of interest occurs.
#define SIGEV_THREAD // A notification function is called to perform notification.
#define SIGALRM // Alarm clock
#define SIGBUS  // Access to an undefined portion of a memory object or invalid address alignment
#define SIGCHLD // Child process terminated, stopped or continued
#define SIGCONT // Continue executing, if stopped
#define SIGHUP  // Hangup
#define SIGKILL // Kill (cannot be caught or ignored)
#define SIGPIPE // Write on a pipe with no one to read it
#define SIGQUIT // Terminal quit signal
#define SIGSTOP // Stop executing (cannot be caught or ignored)
#define SIGTSTP // Terminal stop signal
#define SIGTTIN // Background process attempting read
#define SIGTTOU // Background process attempting write
#define SIGUSR1 // User-defined signal 1
#define SIGUSR2 // User-defined signal 2
#define SIGPOLL // Pollable event
#define SIGPROF // Profiling timer expired
#define SIGSYS  // Bad system call
#define SIGTRAP // Trace/breakpoint trap
#define SIGURG  // High bandwidth data is available at a socket
#define SIGVTALRM // Virtual timer expired
#define SIGXCPU // CPU time limit exceeded
#define SIGXFSZ // File size limit exceeded
#define SIG_BLOCK // The resulting set is the union of the current set and the signal set pointed to by the argument set.
#define SIG_UNBLOCK // The resulting set is the intersection of the current set and the complement of the signal set pointed to by the argument set.
#define SIG_SETMASK // The resulting set is the signal set pointed to by the argument set.
#define SA_NOCLDSTOP // Do not generate SIGCHLD when children stop or stopped children continue.
#define SA_ONSTACK // Causes signal delivery to occur on an alternate stack.
#define SA_RESETHAND // Causes signal dispositions to be set to SIG_DFL on entry to signal handlers.
#define SA_RESTART  // Causes certain functions to become restartable.
#define SA_SIGINFO  // Causes extra information to be passed to signal handlers at the time of receipt of a signal.
#define SA_NOCLDWAIT  // Causes implementations not to create zombie processes or status information on child termination. See sigaction(3p).
#define SA_NODEFER  // Causes signal not to be automatically blocked on entry to signal handler.
#define SS_ONSTACK  // Process is executing on an alternate signal stack.
#define SS_DISABLE  // Alternate signal stack is disabled.
#define MINSIGSTKSZ // Minimum stack size for a signal handler.
#define SIGSTKSZ    // Default size in bytes for the alternate signal stack.
#define SIGTRAP // Process breakpoint or Process trace trap 
#define SIGCHLD // Child has exited or Child has terminated abnormally and did not create a core file or
        // Child has terminated abnormally and created a core file or Traced child has trapped or Child has stopped or Stopped child has continued
#define SIGPOLL // Data input available or Output buffers available or Input message available or I/O error or High priority input available or Device disconnected.                                            
#define Any // Signal sent by kill or Signal sent by sigqueue() or Signal generated by expiration of a timer set by timer_settime() or Signal generated by
            // completion of an asynchronous I/O or request or Signal generated by arrival of a message on an empty message or queue
#define SIGCHLD // Child process ID or If si_code is equal to CLD_EXITED, then si_status holds the exit value of the process; otherwise, it is equal to the signal that caused the process to change state.
        // The exit value in si_status shall be equal to the full exit value (that is,the value passed to _exit(), _Exit(), or exit(), or returned from main()); it shall not be limited to the
        // least significant eight bits of the value. Real user ID of the process that sent the signal.
#define SIGPOLL // Band event for POLL_IN, POLL_OUT, or POLL_MSG

#include <setjmp.h>
/* jmp_buf - type is an array type suitable for storing information to restore a calling environment. The stored information is sufficient to restore execution at the correct
 * block of the program and invocation of that block. The state of floating-point status flags, or open files, or any other data is not stored in an object of type jmp_buf */
typedef jmp_buf;
/* setjmp - saves the current execution context into a variable @env of type jmp_buf. This variable can later be used to restore the current execution context by longjmp function.
 * That is, when a call to longjmp function is made, the execution continues at the particular call site that constructed the jmp_buf variable passed to longjmp.
 * In that case setjmp returns the value passed to longjmp. longjmp is intended for handling unexpected error conditions where the function cannot return meaningfully.
 * This is similar to exception handling in other programming languages */
#define setjmp(env)
/* longjmp - (implementation since C23) Loads the execution context @env saved by a previous call to setjmp. This function does not return.
 * Control is transferred to the call site of the macro setjmp that set up @env. That setjmp then returns the value, passed as the status.
 * If the function that called setjmp has exited (whether by return or by a different longjmp higher up the stack), the behavior is undefined.
 * In other words, only long jumps up the call stack are allowed. (since C11) Jumping across threads (if the function that called setjmp was
 * executed by another thread) is also undefined behavior */
[[noreturn]] void longjmp(jmp_buf env, int status);

