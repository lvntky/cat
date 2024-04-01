#include "../include/thread_pool.h"

void *thread_pool_worker_thread(void *arg)
{
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
	if (number_of_threads == NULL ||
	    number_of_threads < THREAD_POOL_MIN_LIMIT) {
		number_of_threads = THREAD_POOL_MAX_LIMIT;
		return 0;
	}

	if (number_of_threads > THREAD_POOL_MAX_LIMIT) {
		number_of_threads = THREAD_POOL_MAX_LIMIT;
		return 0;
	}

	return -1;
}

thread_pool_t *thread_pool_init(int number_of_threads)
{
	if (thread_pool_limit_handler(number_of_threads) != 0) {
		// TODO: log error here
		return NULL;
	}

	thread_pool_t *thread_pool = malloc(sizeof(thread_pool_t));
	thread_pool->number_of_threads = number_of_threads;
	thread_pool->threads = malloc(number_of_threads * sizeof(pthread_t));
	// TODO: initialize new components of pool

	if (thread_pool->threads == NULL) {
		// TODO: failure, log error on here
		free(thread_pool);
		return NULL;
	}

	for (int i = 0; i < number_of_threads; i++) {
		int result = pthread_create(thread_pool->threads[i], NULL,
					    thread_pool_worker_thread,
					    (void *)thread_pool);
		if (result != 0) {
			// TODO: failure, log here
			for (int j = 0; j < i; j++) {
				pthread_cancel(thread_pool->threads[i]);
			}
			free(thread_pool->threads);
			free(thread_pool);
			return NULL;
		}
	}

	return thread_pool;
}