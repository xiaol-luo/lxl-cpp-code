#include "try_sum.hpp"
#include <stdio.h>

namespace try_sum
{
	void test()
	{
		printf("------ try_sum::test begin \n");
		double ret = 0;
		{
			auto tmp = try_sum::t_sum(1, 1.3f, 3, 3, 1.2f, 1.2f, 1.3f, 4, 4, 5);
			ret = tmp;
			printf("t_sum floats ret = %f\n", ret);
		}

		{
			auto tmp = try_sum::t_sum('a', 'b', 'c', 12, 4.2f);
			ret = tmp;
			printf("t_sum chars ret = %f\n", ret);
			printf("------ try_sum::test end \n");
		}

		{
			auto tmp = try_sum::t_sum_new(1, 2.0f);
			ret = tmp;
		}
	}
}