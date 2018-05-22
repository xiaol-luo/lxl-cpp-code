#pragma once
#include <stdio.h>

template <typename T, int N>
void extract_array_size(T(&xx)[N])
{
	int n = N;
	printf("extract_array_size : n = %d\n", n);
}

template <typename T>
void try_ref(T const & x)
{
	printf("x in try_ref(T const &): %s\n", typeid(x).name());
}

template <typename T>
void try_nonref(T x)
{
	printf("x in try_nonref(T x): %s\n", typeid(x).name());
}

template <typename T>
void print_ref(T & x)
{
	printf("print_ref %s \n", typeid(T).name());
}

template <typename T>
void print_ref(const T & x)
{
	printf("print_ref_const %s \n", typeid(T).name());
}

template <typename T>
void print_ref(volatile T & x)
{
	printf("print_ref_volatile %s \n", typeid(T).name());
}

template <typename T>
void print_ref(const volatile T & x)
{
	printf("print_ref_const_volatile %s \n", typeid(T).name());
}

template <typename T>
void print_ptr(T *t)
{
	printf("print_ptr %s \n", typeid(T).name());
}

template <typename T>
void print_ptr(const volatile T *t)
{
	printf("print_ptr_const_volatile %s \n", typeid(T).name());
}

template <typename T>
void print_ptr(const T *t)
{
	printf("print_ptr_const %s \n", typeid(T).name());
}

template <typename T>
void print_ptr(volatile T *t)
{
	printf("print_ptr_volatile %s \n", typeid(T).name());
}
