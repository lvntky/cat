#include "../include/thread_pool.h"

void *thread_pool_worker_thread(void *arg)
{
	// Cast the argument to the thread pool structure
	thread_pool_t *pool = (thread_pool_t *)arg;

	// Worker thread main loop
	while (1) {
		// TODO: Retrieve a task from the task queue or task list
		// (Ensure thread safety when accessing shared data structures)

		// TODO: Execute the retrieved task

		// TODO: Handle task completion or errors

		// For illustration purposes, here we just print a message
		printf("Worker thread %ld: Executing task...\n",
		       pthread_self());

		// Simulate some work by sleeping for a short duration
		usleep(100000); // Sleep for 100 milliseconds (100000 microseconds)

		// For demonstration, worker threads will continue to execute tasks until cancelled
		// In a real-world scenario, you'd implement logic for stopping the thread when necessary
	}

	// The worker thread will never reach here, as it's designed to run indefinitely
	// However, a return statement is added to avoid compiler warnings
	return NULL;
}

/**
 * @brief handle given number_of_threads accordingly for various cases
 * 
 * @param number_of_threads 
 */
int thread_pool_limit_handler(int *number_of_threads)
{
	if (number_of_threads == NULL) {
		return RETURN_FAILURE; // Invalid input
	}

	if (*number_of_threads < THREAD_POOL_MIN_LIMIT) {
		*number_of_threads = THREAD_POOL_MIN_LIMIT;
	} else if (*number_of_threads > THREAD_POOL_MAX_LIMIT) {
		*number_of_threads = THREAD_POOL_MAX_LIMIT;
	}

	return RETURN_SUCCESS; // Success
}

thread_pool_t *thread_pool_init(int number_of_threads)
{
	// Check and handle thread limit
	if (thread_pool_limit_handler(&number_of_threads) != 0) {
		fprintf(stderr, "Error: Invalid number of threads.\n");
		return NULL;
	}

	// Allocate memory for the thread pool
	thread_pool_t *thread_pool = malloc(sizeof(thread_pool_t));
	if (thread_pool == NULL) {
		fprintf(stderr,
			"Error: Memory allocation failed for thread pool.\n");
		return NULL;
	}

	// Initialize number of threads
	thread_pool->number_of_threads = number_of_threads;

	// Allocate memory for the array of pthreads
	thread_pool->threads = malloc(number_of_threads * sizeof(pthread_t));
	if (thread_pool->threads == NULL) {
		fprintf(stderr,
			"Error: Memory allocation failed for threads.\n");
		free(thread_pool);
		return NULL;
	}

	// Create worker threads
	for (int i = 0; i < number_of_threads; i++) {
		int result = pthread_create(&thread_pool->threads[i], NULL,
					    thread_pool_worker_thread,
					    (void *)thread_pool);
		if (result != 0) {
			fprintf(stderr, "Error: Failed to create thread %d.\n",
				i);
			// Clean up resources and return NULL
			for (int j = 0; j < i; j++) {
				pthread_cancel(thread_pool->threads[j]);
			}
			free(thread_pool->threads);
			free(thread_pool);
			return NULL;
		}
	}

	return thread_pool;
}

void thread_pool_destroy(thread_pool_t *pool)
{
	if (pool != NULL) {
		free(pool->threads);
		free(pool);
	}
}

void thread_pool_submit(thread_pool_t *pool, void (*task)(void *), void *arg)
{
}

int thread_pool_get_size(thread_pool_t *pool)
{
	return pool->number_of_threads;
}