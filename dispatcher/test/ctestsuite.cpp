#include <cstring>

#include "ctestsuite.h"
#include "tests/ctesttimer.h"

bool CTestSuite::ProcArgument(const char* arg)
{
    bool test_ok = true;
	CTestCase* tst = nullptr;
    if      (strcmp(arg, "timer") == 0)		tst = new CTestTimer();
    if (tst)
	{
		sTestPerformance test_perf;
		test_ok = tst->Test(test_perf);
        printf("%s==========================\n", test_perf.test_name.c_str());
        printf("==== TEST %s PERFOMANCE ====\n", test_perf.test_name.c_str());
		for (const sTestPerformanceAspect &tpa : test_perf.perfomance)
		{
            printf("= %s\n", tpa.Print().c_str());
		}
        printf("%s==========================\n", test_perf.test_name.c_str());
	}
    return test_ok;
}
