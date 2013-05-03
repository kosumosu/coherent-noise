#pragma once

#include <cmath>
#include <type_traits>

namespace noise
{
	template <typename TArray, typename TFirst, typename ... TRest>
	void setValues(TArray array[], const TFirst & first, const TRest & ... rest)
	{
		array[0] = TArray(first);
		setValues(array + 1, rest ...);
	}

	template <typename TArray, typename TFirst>
	void setValues(TArray array[], const TFirst & first)
	{
		array[0] = TArray(first);
	}

	template <typename T>
	T saturate(const T & value, const T & min_value, const T & max_value)
	{
		return std::min(max_value, std::max(min_value, value));
	}

	template <typename TInt, typename TFloat>
	TInt fast_floor(const TFloat & float_value)
	{
		static_assert(std::is_floating_point<TFloat>::value, "Argument must be floating point!");
		static_assert(std::is_integral<TInt>::value && !std::is_same<TInt, bool>::value, "Return value must be of integral type but not bool.");
		return float_value >= TFloat(0) ? static_cast<TInt>(float_value) : static_cast<TInt>(float_value) - TInt(1);
	}
}