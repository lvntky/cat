#include <cmocka.h>
#include <setjmp.h>

void *mock_thread_pool_worker_thread(void *arg)
{
	// Mock implementation for testing purposes
	// return NULL;
}

int main()
{
	const struct CMUnitTest tests
		[] = { cmocka_unit_test(mock_thread_pool_worker_thread) }

	return cmocka_run_group_tests(tests, NULL, NULL);
}