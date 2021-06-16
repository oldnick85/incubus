#include "ctesttimer.h"

bool CTestTimer::Test(sTestPerformance &test_perf)
{
	printf("==== Testing strings ====\n");
	test_perf.test_name = "STRING";

	printf("= \n");
	test_perf.perfomance.emplace_back();
	sTestPerformanceAspect &perf_div = test_perf.perfomance.back();
	perf_div.name = "";
	perf_div.Start();

	perf_div.End();

	return true;
}
