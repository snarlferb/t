### #Implementation#, represents those mechanisms i plan to implement

## Prelude
When we talk about parallelizing code, the approach for general applications differs significantly
from parallelizing tasks in numerical computing, such as machine learning. In general applications,
the goal is to leverage **multithreading** and manage synchronous or asynchronous behavior. This involves
identifying **independent processes** that can be executed concurrently as threads, and ensuring they **don't
interfere with each other**. Proper management of **shared resources** between threads is essential, often
requiring tools like condition variables, mutexes, or other synchronization mechanisms to ensure thread
safety.

In contrast, in machine learning and numerical computing, the focus is on parallelizing highly repetitive
operations, like matrix and tensor calculations, that can be processed in parallel without much inter-thread
communication. Here, the task is less about coordinating processes and more about optimizing data operations,
with much of the parallelism happening at the level of individual data elements (e.g. a single value in a
row/column of a matrix)

Of course, we'll be talking about more than threads and parallelizing code...

### What to expect
These concepts are hard to explain at first, because there's several components, all happening at once,
and it is hard to define them. We may talk about different procedures being performed inside the kernel
or OS, however this isnt to disregard how those procedures work in other contexts. We have to establish
the groundwork first, before we can observe their influence elsewhere.

All of these things like locks, blocking, etc. are all pretty straightforward to understand—Its when we start
to introduce *true asynchronous execution* (tasks that run independently of the main flow of execution, often
concurrently or in parallel) for multi-core, that we start to see different interactions equate to multiple
paths and **directions**. That should be something to keep in mind.

### Kernel management
We have to take a step back and ask ourselves what is happening in the kernel before we can more naturally
breakdown all of these reciprocally connected terms..

The kernel manage *processes*. Processes are instances of programs in various stages of execution.
They allocate resources such as **CPU time** and **memory** to processes, and schedule them for execution
using scheduling algorithms like round-robin, priority-based scheduling, or FIFO. **Context switching** allows
the kernel to swiftly switch the CPU from one process to another by saving  or storing the current process's
state (context) and restoring the next process's state. The "context" of a process includes: Registers
(e.g., program counter, stack pointer, general-purpose registers), Program state (e.g., where in the code
it was executing), and Memory state (e.g., stack and heap pointers).

The kernel schedules I/O operations (Input/Output). This is a fundamental role in managing how data is
transferred between the **system** and **external devices** (like disks, network interfaces, etc.) or internal
devices (like memory). These operations involve several mechanisms, including buffering, caching, and
asynchronous I/O, to ensure efficiency and proper timing.

### Interrupts
Interrupts are signals that indicate an event that **requires immediate attention** from the kernel. Instead of
the CPU constantly checking if an event has occurred (which would waste processing power), the interrupt
mechanism allows devices or software to notify the CPU only when needed. This way, the kernel can respond
as soon as the event happens, making the system more efficient and responsive. There are two main types of
interrupts: hardware and software...

Hardware and software interrupts allow the kernel to respond to events like I/O completion, hardware errors,
or requests from processes, and in doing so it ensures said event can be processed in a timely manner.

Traps are specifically **software-generated interrupts**, typically caused by an exception or a deliberate
request for kernel services (like a system call). They are synchronous events that tell the program to
interrupt its normal execution and transfer control to the operating system. On the other hand a *signal*
is an asynchronous notification that is sent to a process to inform it of an event.

We can draw a comparison between many different things here; Apropos to our investigation are those
mechanisms that have to do w/ kernel intervention, the *interrupts* that tell a process to *block* temporarily,
and the mechanisms used for coordinating and syncing interactions; **They all effectively pause the flow
of execution of a given process.**

### Processes in the kernel
In the case of the kernel, its job here is to manage concurrency by allowing multiple process to run seemingly
at the same time through a method called time-sharing. It uses the scheduler to **alternate CPU access** between
processes, creating a kind of parallelism. The kernel scheduler allocates and assigns a specific time interval
called a "time slice" to each process. It **rapidly switches** between them, which efficiently balances the load
and ensures system **resources are shared**. Each process that is ready to run is assigned a time slice, during
which it gets exclusive access to the CPU. When its time slice expires, the kernel pauses the process and
assigns the CPU to another process, ensuring fair sharing of resources.

During the assigned time slice, the process can perform any tasks, such as executing code, reading from memory,
or performing I/O. Once the process’s time slice is used up, the kernel interrupts the process (through a timer
interrupt for example). What happens within a processes lifetime?...

For the processes behavior itself, it must first be initialized or created, whereby it'll introspect for a time
(called "running"), waiting for CPU allocation before execution—before it receives an instruction and knows which
CPU core to go to. Once allocated, the process transitions to the *running* state where it actively executes its
tasks, after which it'll *wait* for events.

If a process is ready to run but not currently executing (because another process is using the CPU), the *process
control block (which we'll explain after)* is placed in the ready queue. The kernel then picks the next process in
the ready queue based on a scheduling algorithm (e.g., priority-based or round-robin).

The kernel performs a **context switch** to save the state of the process and load the state of a new process.

The Process Control Block (PCB) is a representation of a process, or what the kernel considers a process, serving
as a repository that stores all the data needed to start or resume a process. As such, the kernel saves the state
of the currently running process into a data structure called the **process control block** like a snapshot, which
ensures that when the interrupt is handled and the process is resumed, it can continue as if nothing happened.

### Processes in general
Upon completion, processes terminate! Therefore, each state of a process will happen in an orderly progression;
Ready, Running, Waiting, Terminated—In priority-based scheduling, higher-priority processes are given CPU time
before lower-priority ones, where as in round-robin scheduling, processes in the ready state are handled in a
circular queue, each getting a fixed time slice. Therefore, processes are queued based on their priority or
other scheduling algorithms.

IPC stands for Inter-Process Communication, and it refers to the methods and mechanisms by which processes
(running programs) can communicate with each other. Some systems utilize shared memory buffers to facilitate
inter-process communication (IPC). Ports, in this context, then act as endpoints that receive and manage said
messages. While a port is often thought of as an abstraction for routing messages between processes, in some
cases, it can be backed by shared memory, where the actual message payload is stored in a common region and
accessed by both sender and receiver. In a multi-process operating system environment, processes often need
to exchange data, synchronize actions, or coordinate tasks. IPC mechanisms provide the means to do those
things, and shared memory, as well as message passing are two fundamental ways that processes or threads
can communicate/coordinate with each other.

Shared memory refers to a region of memory that multiple processes or threads can access simultaneously.
Unlike message passing, which involves explicit sending and receiving of data, shared memory allows direct
modification of data structures in a common address space. This means that communication can be extremely
fast, as no data needs to be copied or transferred—processes or threads simply read and write to the same
location. However, this also introduces the need for synchronization mechanisms, such as mutexes, semaphores,
or atomic operations, to prevent race conditions where multiple entities modify shared data unpredictably.

IPC mechanisms are a complex topic on their own. There are IPC Mechanisms based on shared memory
**Shared memory segments** (shmget, mmap, etc.) which are processes that directly read and write to a
common memory region, or **memory-mapped files** (mmap) that allow multiple processes to access the same
file in memory. Then there are IPC mechanisms based on message passing (Message queues, sockets, signals,
and so on..)

### IO Blocking vs Thread Blocking
Before we can talk about blocking, there's a distinction that should be made first. When we talk about blocking
in the context of threads, we are referring to a thread's execution being halted or **paused** while it waits
for some condition or **resource to become available**. In this context, blocking typically occurs during
synchronization operations, such as when a thread is waiting for a lock or a signal from another thread.

Blocking in the context of I/O operations, however, is a different concept. It refers to how input/output
operations behave when accessing data. In this context, blocking refers to the **predictability and completion
of read/write operations**—For example, in blocking mode, when reading from a file or data stream, the operation
will wait until the requested amount of data is available before returning.

This means if you request say 64 bytes of data, the operation will block until all 64 bytes are read from the
source, or until it’s sure that the requested data can be provided. In non-blocking mode, the read or write
operation will return immediately, even if the full amount of data requested isn’t yet available. If you
request 64 bytes and only 8 bytes are available, it will return those 8 bytes immediately and you’ll need
to perform additional operations to retrieve the rest of the data.

In our case, the former scenario we are most interested in, that is the blocking of a thread(s) (of a given
process) until resources are made available—whereby they must wait for some event to occur. Therefore we will have
to understand some things. For example, there's **read-side/write-side critical sections** for managing access to
shared resources and ensuring data consistency, and other such things.

### Multitasking and threads
First lets go back in time, back in the earliest days of the first browsers for example. Early versions of those
browsers used a single process with multiple threads for rendering and execution, where tabs were just threads them-
selves within a single process. The downside to this approach was that a crash, or an unresponsive event in one tab
could single-handedly crash the entire browser. Most modern browsers now isolate tabs into separate processes rather
than as threads, but it just goes to show the delicate nature of threads.

Producer-consumer problem: One thread (the producer) generates data and another thread (the consumer) processes it.
The shared resource (often a queue or buffer) holds the data between the two threads. Threads can split up work on
a larger dataset, where they each access and modify portions of the shared data. This is called **task parallelism**.

In multitasking environments, we tend to describe more granular aspects of a process—and happens to be out main concern,
that is whether or not a process has a read-side critical section (where shared resources are only read, NOT modified)
or a write-side critical section (where shared resources are being modified) But ultimately a shared resource could be
anything. Take the following for example...
```c
  int shared_counter = 0; // Shared resource
  mtx_t mutex; // Mutex for synchronizing access to the shared resource
```
In situations where you plan to make use of threads like this, it implies that atleast **one thread is going
to be prioritized**, and all others will be blocked. This might not sound useful—compared to running procedurally—
but you should think of it more like a **priority queue**. Threads can be **scheduled**, and thus, the CPU scheduler
**allocates time to each thread**, allowing them to run for a certain period before blocking, and then switching to
other threads. It can also assign different priority levels, ensuring that higher-priority threads get more CPU time
while lower-priority ones wait their turn. 

Threads within the same process share the **same memory space** and other **resources**, which means they can
access the same data structures and **variables directly**. Processes with threads that share memory between them
introduces concurrency (threads that can access memory at the same time) and this is one of the main reasons
that we are bringing threads into the program, as it will clearly benefit us to use concurrency and manage the
resources of a given process.

### Multithreading
Multi-threaded programs have multiple stacks, one for each thread. Each stack can be **dynamically resized** at runtime
to accommodate varying workloads. Although singular threads are associated with a singlular stack, their initial size
is determined, and thus specified at the time of that thread's creation.

We are also going to explain synchronization mechanisms and how they'll ensure proper coordination between
threads when accessing shared resources. Conceptually, threads within a process can be associated w/ both
**read-side** and **write side critical sections**-depending on the specific synchronization and operation being performed.

A process can have multiple threads. Multiple threads or processes can typically enter a read-side critical section
simultaneously if the synchronization mechanism used (such as a reader-writer lock) allows for **multiple concurrent
readers**. However only one thread or process can enter a write-side critical section at a time to prevent concurrent
modifications, ensuring exclusive access during write operations (we'll elaborate on this further).

When multiple threads access the same memory concurrently it can lead to data races and other race conditions.
A data race occurs when both processes (multiple threads) access the same memory location without proper synchro-
nization and at least one of the accesses is a write operation—in simpler terms, a data race occurs when it has tried
to read memory before the other has finished updating. Therefore, the timing of a given thread's execution is important
to consider when contending w/ synchronous behavior.

One way you may encounter a data race is while using a thread to allocate memory w/ `malloc` while another thread
tries to free the same memory block w/ `free`, or if both threads try to access/modify the same data within a
dynamically allocated memory block. Since `malloc` and `free` are not inherently thread-safe, calling them from multiple
threads concurrently has the possibility of producing issues such as corruption, crashes or undefined behavior.
If multiple threads are managing the same pool of dynamically allocated memory, you need a **synchronization mechanism**
to ensure data consistency and prevent such race conditions.

To summarize, proper synchronization coordinate tasks in such a way where only one thread can access or modify the
shared memory at a time, maintaining integrity of the data. This insurance against interference is possible when using
locks, semaphores, and condition variables.

### Asynchronous
"Asynchronous" refers to the occurrence of events that are **independent** of the main program flow. These may be outside
events such as the arrival of signals, or actions instigated by a program that take place concurrently w/ program execution,
without the program "hanging" (unresponsive to the *wait* for said results)

When you have tasks that may take a significant amount of time to complete or when you want to perform multiple tasks
concurrently without blocking the execution of other tasks, then you'll find that an asynchronous model works for the
intended context. This model is best found in event-driven or **callback-based** contexts to handle **shared memory operations**,
that is, to allow threads to perform the other tasks while waiting for shared memory operations to complete.

"Event-driven" is a paradigm in which the flow of the program is determined by events—such as user actions, sensor outputs,
or messages from other programs. This approach is widely used in applications that need to respond to a variety of inputs or
actions; GUIs, web servers, and real-time systems. Callback-based paradigms encompass functions (callbacks) which are
passed as arguments to other functions and are executed once a certain event or operation has been completed.

Shared memory is a specific type of shared resource. It refers to a memory segment that can be **accessed by multiple
threads or processes**. Shared memory allows for fast communication between threads or processes because they can read
and write to the **same memory locations directly** (allows multiple threads or processes to access and modify the same
data concurrently). This shared memory region allows these processes or threads to communicate and share data efficiently,
facilitating inter-process communication (IPC) or inter-thread communication. Asynchronous is then a paradigm that allows a
program to initiate tasks that can run concurrently, without waiting for each task to complete before starting the next one.

### Thread-Local Variables
First lets go over some basic terminology. **Global variables** are variables declared outside any function within a program.
They are accessible by functions within the same scope as that program or a specific file.

Thread-Local Variables are a special type of variable declared with a thread-local storage specifier. Each thread in a
process has its **own copy of a thread-local variable**, independent of other threads. 

Global variables, accessible by all functions, provide a way to share data across the entire program or a specific file.
However, when dealing with multiple threads, there's a distinction to be made. Thread-local variables offer a mechanism for
each thread to have its own private copy of the data, ensuring no other thread can interfere. In contrast, non-thread-local
global variables (that is global variables in multithreaded environments) become a shared memory space. All threads
within the program can access and modify the same instance of these variables, allowing for communication and
data exchange between threads.

In multi-threaded applications, threads often communicate using uni-directional queues, shared memory, or synchronization
primitives such as mutexes and condition variables. As such, threads introduce an additional layer of complexity in how
blocking is handled within a given process. When a thread performs an operation that causes it to block (e.g., waiting
for I/O, acquiring a lock), only that specific thread is blocked, not the entire process. Other threads within the *same
process* can continue to execute. Each thread has its own state and can independently transition between running, ready, and blocked states.

A thread is just **a unit of execution within a process**, enabling a program to perform multiple tasks seemingly at the same time.
So in the context of concurrency, threads allow a program to manage multiple operations in the overlapping manner we discussed.
Each thread can run a different piece of code, and the operating system can switch between these threads rapidly, creating
the illusion that they are executing simultaneously.

Generally, for multiple threads that contend for access to a shared resource (that which are protected by a lock), only one
thread can hold the lock at any given time. While a thread holds the lock and accesses the shared resource, other threads must
*wait* until the lock is released before they can acquire it and access the resource themselves. This ensures that each task
makes progress without the need for one to finish before another begins. When we talk about something being parallel within the
context of concurrency, we refer to multiple threads actually executing at the same exact moment, typically on different CPU cores.

This is the essence of parallelism, where tasks are not only managed concurrently—but are also physically running simultaneously,
leveraging multi-core processors to speed up computation-intensive operations.

So we mentioned that in order for a thread to access a shared resource, it must first **acquire a lock**. Acquiring a lock means that
the thread requests ownership of the lock to proceed w/ its operation. If the lock is already held by another thread, the requesting
thread is typically blocked (put to sleep effectively) until the lock becomes **available**. Once the lock is available/acquired, the
thread can safely access the shared resource without interference from other threads.

After the thread has finished using the shared resource it must release or unlock the lock. Releasing the lock makes it available for
other threads that might be waiting to acquire it. This ensures that the critical section of code—the part that accesses the shared
resource is **exited safely**, allowing other threads to enter in.

### Atomicity
Atomicity refers to the property of an operation that makes it appear to the rest of the system as if it occurred instantaneously.
In the context of atomic operations (provided by stdatomic.h) atomicity ensures that certain operations on shared variables are
**indivisible**, and uninterruptible. That means that when multiple threads are concurrently accessing/modifying the same memory location,
atomic operations guarantee that these operations will either **all take effect-or none at all**. This prevents race conditions and ensures
consistency in the program's behavior.

Note: This gets us into atomic primitives, memory fances, as well as cache coherency protocols (built into the CPU) in order to better
explain why/how this happens. We'll talk about this more when we explain serialization.

Ordering refers to the sequence in which operations appear to occur with respect to each other. In a concurrent program, operations
performed by different threads might not necessarily execute in the same order in which they are written in the code. However, certain
operations might need to be ordered in specific ways to ensure correct behavior. Atomic operations can provide different levels of
ordering guarantees, such as acquire, release, and sequentially consistent ordering. These ordering constraints ensure that operations
are visible to other threads in a predictable manner, preventing unexpected behaviors due to reordering.

I dont like analogies, however I think the following  "lane intersection"  illustration helps paint the picture of what happens during
scenarios where averting deadlocks is important. Imagine you have pedestrians strolling or walking along, in or near the street or
highway: Pedestrians w/ **less resources** and automobiles w/ **more resources**, i.e. as a  **verbose mechanism**.

And so, they're carrying bags of resources to some destination. They can either move in a **fast lane** or a **slow lane**—Whereby the
intersection represents the **contention point**—a place that is susceptible to deadlocks (deadlocks are a situation where a subsequent
process enters a sequence of **perpetual waiting** because of other threads or processes that also began waiting there. Make sense?)

Each lane represents a distinct area of shared memory where threads can operate independently or interact with each other as needed.

Condition variables are like the traffic lights at a busy intersection, ensuring that threads proceed safely through shared resources.
Initialized with `cnd_init()`, they wait patiently until conditions are right, just like drivers wait for the green light. Threads give
way using `cnd_wait()`, letting others take the lead until it's their turn. When it's time to go, `cnd_signal()` and `cnd_broadcast()` act
like the green light, signaling threads to move forward. Time management is also key, with `cnd_timedwait()` offering a timeout feature,
like a traffic light changing after a set duration.

Finally, `cnd_destroy()` brings closure, causing the blackout-power outage to that highway's intersection so it is no longer needed,
allowing those resources to be freed up until the next time that these tasks matter. Just know that i typically despise analogies, however
i believe it called for a good one, this one and only time.

One of the most common atomic primitives is CMPXCHG (Compare-and-Exchange) in x86, for example, `LOCK CMPXCHG [mem], reg` instruction would
read a memory location, compare it to an expectd value, write a new one (ONLY if it matches), or FAIL, if it doesnt match (ergo nothing happens)

### What is a lock?
A lock is a synchronization primitive that prevents state from being modified or accessed by multiple threads of execution at once.
Locks enforce mutual exclusion (like a mutex) for concurrency control policies. Locks are advisory, whereby each thread cooperates by
acquiring the lock before accessing the corresponding data.

Most locking designs block the execution of the thread requesting the lock until it is **allowed to access the locked resource**.

With a spinlock, the thread simply waits ("spins") until the lock becomes available. This is efficient if threads are blocked for a short
time, because it avoids the overhead of kernel process re-scheduling. It is inefficient if the lock is held for a long time, or if the
progress of the thread that is holding the lock depends on preemption of the locked thread; preemption meaning forestalling or
*to pause temporarily"*, beforehand.

Preemption can occur when a higher-priority task becomes runnable or when the kernel decides to enforce fairness in scheduling by *"preempting"*
those lower-priority tasks. So to put it another way, **preemption** refers to the ability of the kernel to temporarily pause the execution
of a currently running task, by which some other task is then allowed to run.

Choosing a small number of locks that each protect a large segment of data (granular approach) results in less lock overhead when a single
process is accessing the protected data, but worse performance when multiple processes are running concurrently. However the more granular
the lock, the higher the likelihood that the lock will stop an unrelated process from proceeding.

Just in general for multiple threads that are contending for access to a shared resource (**that which are protected by a lock**), only **one
thread can hold the lock** at any given time. While a thread holds the lock and accesses the shared resource, other threads must **wait** until
the lock is released before they can acquire it and access the resource themselves.

When a thread attempts to acquire a lock but finds it unavailable, it may be temporarily suspended (blocked) and placed in a **waiting state**.
One such reason this can happen is due to a context switch/switching, where the CPU switches from executing the blocked thread to executing
another thread that is ready to run. Context switches incur overhead due to saving and restoring thread state. By *overhead*, we mean the
additional computational resources consumed—such as time, memory, or processing power—by the aforementioned synchronization mechanisms.

### Cooperative multitasking
This is a programming technique for simulating the appearance of **multiple tasks running concurrently** within a single thread.
Unlike preemptive multitasking, or multitasking wherein the kernel controls when tasks are switched, cooperative multitasking
relies on the tasks themselves to voluntarily **yield** control.

The application code is divided into smaller sections representing individual tasks. These tasks could be handling user input,
processing data, or waiting for external events. Each task is responsible for managing its own execution time. When a task finishes
its current step or needs to wait for something (like waiting for data to arrive), and it explicitly yields control to the next task
in line. This yielding involves **saving the state** of the current task (registers, variables) and switching to the context of the
next task.

The next task retrieves its saved state and resumes execution from where it left off. This process of task execution, yielding, and
resuming continues, creating the illusion of multiple tasks running simultaneously.

A fiber is a lightweight unit of execution that resembles a thread but is **managed by the application**. Unlike threads, which can be
scheduled by the kernel, fibers rely on the application to decide when to switch between them. This makes fibers more efficient for
certain tasks, as context switching between fibers is typically faster than between threads. If you want to gain more insight into
the differences between user-level and kernel-level threading lookup "threading model".

## Primitives
### Mutex
A mutex refers to a mutual exclusion lock, ensuring only one thread can access the critical section at a time. While atomic primitives are
efficient for simple read-modify-write operations on shared variables, mutexes offer more flexibility and control over **concurrent access
to shared resources**. Mutexes can be implemented w/ built-in support for wait and notify mechanisms, such as condition variables.
These mechanisms allow threads to wait for specific conditions to be met before proceeding, enabling more complex synchronization
patterns and inter-thread communication.

A mutex must be **unlocked by the task which locked it** and the task can **not** lock a **locked mutex** again (we'll talk more about
mutexes later) Spinning occurs when a thread repeatedly checks the **locks status** in a tight loop, until said lock becomes available again.

A spinlock is a lock that a thread keeps trying to acquire until it is successful. Its used for short critical sections, or rather, its for
those situations where the expected wait time (availability) for the lock is short, and where the overhead of putting the thread to sleep
(blocked) and waking it up would be higher than the overhead to keep it spinning.

### Semaphores
Semaphores are a synchronization primitive that control access to **a limited number of resources**, considered useful to manage the access of
those shared resources that require **frequent reads**. While holding a semaphore it is safe for a process to sleep or be scheduled. Therefore,
semaphores are used when the lock may be **held for a long time**. Read-write semaphores are used in situations where it is beneficial to
have multiple readers simultaneously.

**Semaphores** are essentially variables that can be incremented or decremented (atomically) by threads or processes—meaning that those
processes dont contend or interrupt, **and the system decides which goes first**. There's only two ways to appoint a semaphore, `wait()`
or `signal()`, which we will see illustrated below.

Say we have three threads: `A`, `B` and `C`... We also have an `S` for semaphore that holds the count `[  ]`
So imagine a race track with each result hanging where the traffic lights would be, and our counter at the end, which essentially keeps track
of the last or total action that occurred:
```
S:                     [ 1 ]
A: ..waits..           [ 0 ]
B: ... waits for..     [ ..]  .."waitsuccess"
C: ... ... ... post..  [ 1 ] ...... [ 0 ]
```
Thread `S` holds the count at 1. Thread `A`, calls `wait()` and the thread drops from 1 to 0. Thread `B` calls `wait` but cant yet decrement so
it just *..waits..*. Thread `C` calls `signal()` and increments from 0 to 1, which as a result allows Thread `B` to decrement the value to 0,
and then return from wait.

While the ordering of thread execution and semaphore acquisition may be influenced by factors such as scheduling policies, system load, and
timing, atomic operations only secure the predictability of the order (regardless of the interleaving of threads) and not necessarily in
construing which approach you took for a semaphore queue.

If Thread `A` and `B` call *wait* and it occurs at the same time, those standards of atomic operations apply, and the system decides who's first.
In this sense, a semaphore is a more flexible type of mutex. Semaphores maintain a count of available resources and allow tasks to wait for, or
signal the availability of these resources. Unlike mutexes, semaphores may be released by a different task than the one that acquired them
depending on the scenario, where as mutexes function in a simpler, round-about way.

Now we are getting an idea of what, where, and  why  these synchronization primitives are used, and the associated mechanisms they might appear in.

### Workqueues and other mechanisms _#Implementation#_
Workqueues are a mechanism for yielding/scheduling tasks to be executed asynchronously within a worker thread. They are used for handling tasks
that need to be executed **outside** the context of the **current execution flow**, such as deferred work (tasks scheduled for later-different context)
that is in an interrupt context or w/ tasks that need to be performed asynchronously without blocking the thread thats calling out. 

A sleeper is a task waiting mechanism. Its defined by a thread that cannot acquire the lock (because of another threads acquisition of) and it
does so by **voluntarily yielding the CPU**, and thus becomes a "sleeping" thread. The thread remains in the sleeping state until, and the lock
becomes **available**, at which point it is woken up and **granted the lock**. Its typically used in scenarios where the expected wait time for the lock
to become available is relatively long or where spinning would lead to excessive CPU usage.

"Processor local locks" are defined as a lock that is acquired by a thread running on one CPU, where other active threads on the same CPU can
access the lock without causing cache coherence traffic between CPUs. Its beneficial in scenarios where threads **frequently access shared
resources protected by the lock**.

In the context of the linux kernel it helps to work within the symmetric multiprocessor system by giving each CPU its own data structure and
minimizing the need for frequent access to the data structures of other CPUs. This approach can be particularly efficient for tasks like gathering
statistics, where the function simply reads the relevant variable for each CPU and aggregates the results. This method has limitations when the data
structure must be shared across all CPUs. For example, in the case of file operations, where all CPUs need to access and update the same data—such as
the "dcache"—localizing data to individual CPUs is not feasible.

In modern CPUs, when we refer to individual CPUs in the context of symmetric multiprocessor (SMP) systems or multicore processors, we typically mean
CPU cores, not separate physical processors.

### Lock-based *(as well as lock-free mechanisms)*
Lets suppose we're creating a singleton structure design pattern (ensuring that there is only one instance of the structure throughout the lifetime
of the program) and it has some variable, `Data_A`. It'll be responsible for control access to shared resources. Access to the singleton instance is
synchronized using the mutex, i.e. only **one thread** can access a critical section of code at a time, to ensure thread safety. If a thread tries to
acquire a lock that is already held by another thread, it will be blocked until the lock is released. This waiting period is called a "delay" and
was caused by our synchronization.

Delays can occur due to various reasons, such as accessing shared resources protected by locks. These delays can introduce opportunities for the
CPU to reorder memory accesses, to improve performance or utilize idle cycles efficiently. Designing in a way that acknowledges delays—since they
may occur at any moment—is one motive you might have when investigating this determinism. Compiler directives and memory-fence instructions can be
implemented to preserve the ordering.

### Synchronization points
Barriers are synchronization points, wherein all participating threads must arrive prior to any of them proceeding further. If any thread reaches
the barrier, and other threads have not yet arrived, it will be blocked until all threads have reached said barrier, and that barriers' lifted.
They are suitable for scenarios where multiple threads or processes need to **synchronize their progress** and coordinate their actions at specific
points in the program (perhaps in terms of phases) where each phase consists of a **set of tasks that must complete** before moving on to the next phase.

Remember, memory fences (or memory barriers) and "barriers"  are related, but are distinct concepts... Memory fences are a specific type of synchronization
primitive used to enforce ordering constraints on memory operations, while synchronization points (or barriers) for multi-threaded programs offer broader
synchronization that ensures the completion of a group of operations **before** allowing threads to proceed. Fences can be used as apart of synchronization
points to ensure memory operations are properly ordered within a synchronized block of code. *ps. i'll always use the term "memory-fence" so you're not
confused on whether i mean a memory-barrier, or barrier*

Cancellation points are specific points in a program where a thread can be **asynchronously terminated** or canceled. These points allow for the orderly
cancellation of a thread in response to a cancellation request from another thread. They're essential for ensuring that threads can be safely cancelled
without leaving resources in an inconsistent state or causing deadlocks. *They provide a mechanism for threads to respond to cancellation requests while
performing potentially long-running or blocking operations.*
 
A process that is blocked is waiting for some event, such as resource availability or the completion of an I/O operation. Something may be considered
blocked if it has also been locked, however the two terms are not interchangeable. Of course in some situations a "wait" may also be considered as being
"blocked". During a blocking operation, the calling thread is paused until the **operation completes** or a condition is met. This means that when a function
is called, the thread waits until it receives a response or the operation it's waiting for finishes before continuing execution. During this waiting period,
the thread is typically *idle*.

In contrast, non-blocking operations allow the calling thread to continue execution immediately after initiating the operation **regardless of whether
it has completed or not**. The function returns control to the caller immediately, often with a result indicating that the operation is still pending,
or some error condition. Non-blocking operations are often used in scenarios where waiting for completion would cause unnecessary delays or where the
program needs to perform other tasks while waiting for I/O/resource availability.

**Non-blocking I/O** is a technique where I/O operations return immediately, even if the operation isn't complete, allowing the program to continue without
blocking the thread. Polling and asynchronous I/O are common implementations of non-blocking I/O. **I/O multiplexing** is a broader concept encompassing
methods to handle multiple I/O operations efficiently within a single process, typically utilizing non-blocking I/O.

### Grace periods
A grace period is a predefined, limited time window granted by a system or protocol in specific situations. It allows a process or thread to take
necessary action before stricter enforcement occurs (e.g. the node doesn't take any action within the grace period)

If a resource is not claimed within the grace period, it may be marked as available for other nodes to acquire. In cases where a node fails to respond
in time, its connection could be terminated (in the context of said grace period). Similarly, a node might "lease" a resource—such as a file or service—
for a set duration, ensuring exclusive access. Once the lease expires, a **grace period** is often provided to allow the node to renew the lease or gracefully
release the resource before it becomes available to others.

One example of a grace period is during the absence of "readers" which is signaled by the beginning of a grace period—when it is done the copy stage,
the grace period can begin, whereby the deletion (or reclamation) can be invoked—now asynchronous—which means the readers are able to finish their work
without interruption (hence readers can synchronize w/ writers through grace periods, during which, grace period readers will see the old value of the
pointer up until the grace period has ended). A grace period does not necessarily make a distinction on the order between one read-side critical section
and another.

A Mutex in its most basic form includes a **wait-queue and a spinlock** that serializes access to it; Serialization being a process by which we enforce a
particular sequence of execution for accessing shared resources. Wait-queues are analogous to a line of traffic for processes or threads.
These processes are put on hold because they need to wait for a specific event to happen before they can continue executing.

Lock-free mechanisms achieve thread synchronization without using traditional locking mechanisms like mutexes. Within a lockless context you only need to
guarantee that the system makes progress, not necessarily that every thread succeeds immediately. Lock-free is ideal for scenarios with high contention
and frequent updates, however if you need to guarantee that every single thread completes its operation without any delays, then a wait-free approach
might be preferable.

Wait-free approaches guarantee the possibility for every thread to complete its operation in a **bounded number of steps**, regardless of contention.
No thread can be indefinitely delayed by others, where as a lock-free approach ensures that at least one thread will make progress in a finite number
of steps, but does not guarantee that every individual thread will complete its operation immediately. Some threads may be delayed or retried multiple
times due to contention.

In some cases, lock-free algorithms may use wait-queues internally for blocking operations when certain conditions cannot be met in a lock-free manner.
For example, in a lock-free queue, if a dequeue operation cannot proceed because the queue is empty, the thread may wait on a **wait-queue until an elemen
is added** (enqueued). Some synchronization mechanisms combine both lock-based and lock-free techniques to achieve optimal performance.

### Drawbacks of serialization
We mentioned how atomic operations on shared memory locations by multiple threads can introduce serialization (operations that've executed in step-by-step,
sequential order) potentially negating the benefits of multithreading. Atomic operations and synchronization are in this sense at odds with each other.
Likewise, blocking would defeat the purpose of atomicity because the operation would no longer be guaranteed to execute without interference from other
threads or interrupt handlers. There are several ways to manage this situation and make it more efficient.

One would be to minimize the critical section or the portion of code requiring atomic operations. By making the atomic operation itself as small and
efficient as possible it could reduce the time that other threads are blocked for. We can use lock-free data structures like **wait-free queues** or
**treiber stacks**. Unlike traditional queues that rely on locks to ensure data integrity, wait-free queues operate without needing locks, offering
more granular control over a situation.

The basic principle for the treiber stack algorithm is to only add something new to the stack once you know the item you are trying to add is the only thing
that has been added since you began the operation. This is done by using **compare-and-exchange**. Pushing an item to the stack is done by first taking the top of
the stack (old head) and placing it after your new item to create a new head. You then compare the **old head** to the **current head**. If the two are matching then
you can swap the old head to the new one. If not, then it means another thread has added an item to the stack, in which case you must try again. When popping
an item from the stack, before returning the item, you must check that another thread has not added a new item since the operation began. These structures
eliminate the need for explicit locks, improving performance in scenarios with high contention (busy intersection)

In summary, atomic compare-and-exchange attempts to atomically **swap** a value in memory w/ a new value, only if the current value **matches a specified expected value**.
This allows for optimistic concurrency, where threads only incur overhead if theres contention. If your workload allows for it, one can partition the data
among threads such that each thread primarily operates on its own section. This reduces the likelihood of concurrent access to the same memory location.

### Priority Inversion _#Implementation#_
Priority inversion is a scenario in concurrent programming where a low-priority task holds a resource **needed by a high-priority task**, effectively causing the
high-priority task to wait longer than necessary. This occurs when a higher-priority task is blocked, waiting for a lower-priority task to release a shared
resource, but the lower-priority task is itself blocked by another task with intermediate priority. As a result, the high-priority task is effectively **inverted**
in terms of priority as it waits for the lower-priority task to complete its execution and release the resource.

There's several ways we could go about handling priority inversion. Priority-based scheduling policies can be implemented to ensure that **higher priority tasks
are given precedence** over lower-priority tasks; Adjusting the scheduling parameters of threads or processes using functions provided by `threads.h` or system calls
from  `sched.h`  to assign appropriate priorities based on task importance.

We could also implement the **priority inheritance protocol** — A low-priority task holding a mutex inherits the priority of the highest-priority task waiting for the
mutex. This ensures that the low-priority task releases the mutex promptly, preventing higher-priority tasks from being delayed. Alternatively, you might consider using
the **priority ceiling protocol** instead, where each mutex is associated with a priority ceiling value equal to the highest priority of any task that can lock the mutex.
This prevents lower-priority tasks from preempting higher-priority tasks holding the mutex.

### Other type primitives

gettid   — gets the thread ID function

_raw_    — indicates a lower-level primitive that might require more careful handling.

refcount — is the traditional method of walking through a tree where each inode (index node, or data structure representing a directory/file) being accessed is
reference counted. This means that each inode keeps track of how many processes or data structures are currently using it.

rwlock   — reader-writer locks offer a balance between read performance and write consistency, allowing concurrent read access from multiple threads or resources,
and guaranteeing exclusive access for write operations which prevents data corruption where multiple resources try to have mutual access to the data simultaneously.

By allowing only one thread or process to enter the write-side critical section at a time, exclusive access ensures that modifications occur in a serialized fashion.
This serialization of write operations maintains the illusion of atomicity, ensuring that each modification appears to occur instantaneously and independently of other
operations. Exclusive access prevents concurrent conflicts between threads or processes that attempt to modify the same shared resource simultaneously. By enforcing
mutual exclusion, exclusive access guarantees that modifications are applied sequentially, avoiding contention and preserving the consistency of the shared resource.

Unlike mutexes, which enforce exclusive access to shared resources, reader-writer locks provide separate locks for readers and writers. This separation is what is
responsible for the reduced contention between readers and writers, as readers do not block each other and can access the resource simultaneously. They also can
prioritize reader threads over writer threads to prevent **writer starvation**. Reader threads are thus allowed to acquire the lock as long as there are no writer threads
waiting to acquire it, ensuring that readers can access the resource promptly.

RCU (Read-copy update) is more efficient than RW locks for cases where workloads can have many readers and few writers. It avoids situations where threads attempting to
write to a shared resource are continuously blocked and unable to make progress (indefinite wait periods) And it scales well w/ a large number of CPUs (cores)

Reference counting is a memory management technique used to track the number of references or pointers to a resource, such as an object or a block of memory. The basic
idea behind reference counting is to keep track of how many references exist to a particular resource and to free the resource once the count reaches zero. We can look
at it in terms of steps:

When a resource is created or allocated, its reference count is initialized to "1". This initial reference indicates that the resource is being used by at least one part
of the program. Whenever a new reference is assigned to another variable or data structure, the reference count is incremented by 1 again. When a reference to the resource
is no longer needed, such as when a pointer goes out of scope or is explicitly set to NULL, the reference count is decremented by 1.

If the reference count reaches zero after a decrement operation, it indicates that no parts of the program are using the resource anymore. In this case, the resource is
considered no longer needed and can be safely deallocated (freed) *note: e.g. `__counted_by(refcount)` is an attribute thats often used for annotating variables that are
reference-counted by another variable or data structure, which ensures proper concurrency management and memory safety*

