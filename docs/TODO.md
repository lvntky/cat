# CAT ToDo List

- [ ] Thread Pool Management
  - [x] Thread pool manager that can dynamically manage a pool of worker threads.
  - [x] Functions for creating, resizing, and destroying thread pools.
  - [x] Functionality for submitting tasks to the thread pool for execution
  - [ ] Dynamic Task queue size: dynamic resizing mechanism for the task queue to handle varying workloads
  - [ ] Task priority
  - [ ] Task Completion callbacks
  - [ ] More advanced Error Handlings
  - [ ] Task Timeouts
  - [ ] Resource limiter
- [ ] Asynchronous Task Execution
  - [ ] Mechanism for executing tasks asynchronously without explicitly creating threads.
  - [ ] Interface for scheduling tasks to run in the background and providing callbacks upon completion.
- [ ] Advanced Synchronization Mechanisms
  - [ ] Research and design advanced synchronization mechanisms that offer higher-level abstractions than mutexes and condition variables.
  - [ ] Consider implementing features like reader-writer locks, recursive locks, or barriers to address more complex synchronization requirements.
- [ ] Interrupt Handling
  - [ ] Implement functionality for handling interrupts or signals within threads.
  - [ ] Design an interface for registering interrupt handlers and managing their execution in a multithreaded environment.
- [ ] Thread Affinity and Scheduling
  - [ ] Develop functions for controlling thread scheduling policies, priorities, and CPU affinities.
- [ ] Error Handling and Logging Enhancements
  - [ ] Enhance error handling mechanisms to provide more detailed error reporting and diagnostics.
  - [ ] Implement logging functionality to facilitate debugging and troubleshooting of multithreaded applications, including support for logging from multiple threads concurrently.
- [ ] Testing and Validation
  - [ ] Write test cases to cover various scenarios and edge cases, ensuring robustness and correctness under different conditions.
  - [ ] Use valgrind for check memory leaks.
- [ ] Documentation and Examples
  - [ ] Utilize Doxygen for documentation.