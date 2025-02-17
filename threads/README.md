# threads

We'll be summarizing such things as synchronization primitives, asynchronous, atomicity, lock-based and lock-free
mechanisms, as well as blocking and non-blocking in the context of concurrent systems...

There's also the headers `threads.h` and `stdatomic.h` used in the context of a C11—C23 threads library
Other libraries such as `unistd.h`, `sched.h` and `sys/wait.h` can be used for fine-grained control over the
thread creation and stack sizing process, and any subsequent attribute objects. Anything w/ #Implementation#
indicates my own functions using the given library and primitives.

...
