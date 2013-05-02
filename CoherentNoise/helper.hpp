#pragma once

#include <cmath>


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
}