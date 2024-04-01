#include "../include/thread_pool.h"

void *thread_pool_worker_thread(void *arg)
{
	// TODO: MAIN WORKER THREAD FUNCTION
	printf("LOG: %s\n", "you are on thread_pool_worker_thread()");
	return NULL;
}

thread_pool_t *thread_pool_init(int number_of_threads)
{
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