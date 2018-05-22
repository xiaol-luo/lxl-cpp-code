#pragma once

#include <type_traits>

namespace try_sum
{
	// trait sum return value
	template <typename T>
	struct sum_type_trait
	{
		using return_type = std::remove_reference_t<T>;
	};
	// sum of chars return type need to be int, or it will overflow.
	template <>
	struct sum_type_trait<char>
	{
		using return_type = int;
	};
	// sum of chars return type need to be int, or it will overflow.
	template <>
	struct sum_type_trait<short>
	{
		using return_type = int;
	};
	// sum of float return type need to be double, or it will overflow.
	template <>
	struct sum_type_trait<float>
	{
		using return_type = double;
	};
	// etc...

	// common return type
	template <typename T, typename... Args>
	struct sum_return_common_type
	{
		using return_type = std::common_type_t<typename sum_type_trait<T>::return_type, typename sum_return_common_type<Args...>::return_type>;
	};
	template <typename T>
	struct sum_return_common_type<T>
	{
		using return_type = typename sum_type_trait<T>::return_type;
	};

	// sum function implement
	template <typename T, typename... Args> inline
	typename sum_return_common_type<T, Args...>::return_type t_sum(const T& t, const Args&... args)
	{
		// static_assert(all_types_same<T, Args...>::value, "all input params need to be same");
		return t + t_sum(args...);
	}
	// sum function partial specialization
	template <typename T> inline
	typename std::remove_reference_t<T> t_sum(const T& t)
	{
		return t;
	}

	template <typename... Args>
	typename sum_return_common_type<Args...>::return_type t_sum_new(const Args&... args)
	{
		using return_type = typename sum_return_common_type<Args...>::return_type;
		return_type ret = return_type();
		return ret;
	}

	extern void test();

	// not use
	// check whether all inputs are the same type
	template <typename T, typename U, typename... Args>
	struct all_types_same
	{
		static int const value = all_types_same<T, U>::value && all_types_same<U, Args...>::value;
	};
	// check whether all inputs are the same type. partial specialization
	template <typename T, typename U>
	struct all_types_same<T, U>
	{
		static bool const value = std::is_same_v<T, U>;
	};
}