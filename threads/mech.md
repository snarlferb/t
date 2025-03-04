Classification, and which class of mechanisms: First document which mechanisms are worth documenting.
Then come up with a way to look at each set of primitives in terms of grouped mechanisms that work
for specific use cases. Until then we wont have an irrefutable plan to look at...

### Basic Mutexes and File Locks:

- **`1_xxx.c`**: Basic mutexes and file locks
  - **`1_mtx.c`**: Mutex locks using `threads.h`
  - **`1_file.c`**: File locks using `@fcntl` or `@flock`

### Advanced Lock Mechanisms:

- **`2_xxx.c`**: More advanced lock mechanisms like reader-writer locks and priority inversion handling
  - **`2_1_rw.c`**: (e.g. variables `@write_lock`)
  - **`2_2_rw.c`**: Reader-writer locks (if implemented)
  - **`2_prio_mtx.c`**: Priority inversion-safe mutexes (if applicable)

### Process Synchronization:

- **`3_xxx.c`**: Locks and synchronization mechanisms related to process management
  - **`3_procS.c`**: Process synchronization using wait and fork
  - **`3_wq.c`**: Implementation of a workqueue for task management
  - **`3_event_bus.c`**: Implementation of an event bus for event handling

