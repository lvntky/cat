#include "../include/thread_pool.h"

// check it
void *thread_pool_worker_thread(void *arg)
{
	thread_pool_t *thread_pool = (thread_pool_t *)arg;
	// TODO: MAIN WORKER THREAD FUNCTION
	printf("LOG: %s\n", "you are on thread_pool_worker_thread()");
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
		return -1; // Invalid input
	}

	if (*number_of_threads < THREAD_POOL_MIN_LIMIT) {
		*number_of_threads = THREAD_POOL_MIN_LIMIT;
	} else if (*number_of_threads > THREAD_POOL_MAX_LIMIT) {
		*number_of_threads = THREAD_POOL_MAX_LIMIT;
	}

	return 0; // Success
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