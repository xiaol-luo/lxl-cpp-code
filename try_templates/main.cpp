#include <stdio.h>
#include <typeinfo>
#include "examples/try_sum.hpp"
#include "examples/EventDispacher.h"
#include <functional>
#include "examples/try_const.h"

struct FiveInt : std::integral_constant<int, 5> {};

template <typename T>
void check_type(T t)
{
	printf("check_type t : %s\n", typeid(t).name());
	printf("check_type T : %s\n", typeid(T).name());
}

template <typename T>
void check_type(T *t)
{
	printf("check_type t : %s\n", typeid(t).name());
	printf("check_type T : %s\n", typeid(T).name());
}

template <typename... Args, typename binder>
void subscribe(int id, binder b)
{
	std::function<void(Args...)> x = b;
	printf("binder function go here \n");
}

template <typename return_type, typename... Args>
void subscribe(int id, std::function<return_type(Args...)>& f)
{
	std::function<void(Args...)> x = f;
	printf("std::function go here \n");
}

template <typename return_type, typename... Args>
void subscribe(int id, return_type(*f)(Args...))
{
	std::function<void(Args...)> x = f;
	printf("common function go here \n");
}

class A
{
public:
	static int test_static(int a) { return 0; }
	int test_bind(int a, int b, int c) { return 0; }
};

void test_void() {}

int main(int argc, char **argv)
{
	try_sum::test();

	printf("fiveInt::value is %d\n", FiveInt::value);

	int a = 0;
	int *p = &a;
	check_type(a);
	check_type(p);

	std::function<int(int, int)> sum_two_int = try_sum::t_sum<int, int>;
	subscribe(1, sum_two_int);
	subscribe(2, try_sum::t_sum<int, int>);
	subscribe(3, A::test_static);

	{
		A ax;
		auto b = std::bind(&A::test_bind, ax, 2, 2, std::placeholders::_1);
		std::function<void(int, float)> axf = b;
		subscribe<int>(4, b);
	}

	{
		EventDispacher ed;
		ed.subscribe(1, try_sum::t_sum<int, int>);
		ed.subscribe(2, A::test_static);
		ed.subscribe(3, sum_two_int);
		A ax;
		ed.subscribe<int, int, float>(4, std::bind(&A::test_bind, ax, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3));
		ed.subscribe(5, test_void);
		std::function<void()> f(test_void);
		ed.subscribe(5, f);

		ed.Fire(1, 2, 3);

		ed.Fire(2, 10);
		ed.Fire(3, 20, 21);
		ed.Fire(4, 31, 32, 33.0f);
		ed.Fire(5);
	}

	{
		try_sum::sum_return_common_type<int, int, char>::return_type xx;
		try_sum::sum_return_common_type<int, float, double>::return_type yy;
		std::common_type_t<char, char, char> ggg;
		int a = 0;
		++a;
	}

	{
		char buff[1024];
		extract_array_size(buff);
		try_ref(buff);
		try_nonref(buff);

		{
			const volatile int a = 0;
			volatile int b = 0;
			const int c = 0;
			int d = 0;
			print_ref(a);
			print_ref(b);
			print_ref(c);
			print_ref(d);
		}
		{
			const volatile int * a = 0;
			volatile int *b = 0;
			const int *c = 0;
			int *d = 0;
			print_ptr(a);
			print_ptr(b);
			print_ptr(c);
			print_ptr(d);
		}

		int a = 0;
		const volatile int *b = nullptr;
	}
}