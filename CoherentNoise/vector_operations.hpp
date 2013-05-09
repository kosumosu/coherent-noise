#pragma once

#include "vector.hpp"

#include <limits>

namespace noise
{
	template <typename TSpace, std::size_t DIMENSIONS>
	TSpace length2( const vector<TSpace, DIMENSIONS> & vec )
	{
		TSpace squared_length = TSpace(0);

		details::index_iterator<0, DIMENSIONS - 1>::iterate([&] (size_t i)
		{
			squared_length += vec[i] * vec[i];
		});

		return squared_length;
	}

	template <typename TSpace, std::size_t DIMENSIONS>
	TSpace length( const vector<TSpace, DIMENSIONS> & vec )
	{
		return std::sqrt(length2(vec));
	}

	template <typename TSpace, std::size_t DIMENSIONS>
	vector<TSpace, DIMENSIONS> normalize( const vector<TSpace, DIMENSIONS> & vec )
	{
		static_assert(std::numeric_limits<bool>::is_integer, "Can't normalize integer vector.");

		TSpace inversed_length = TSpace(1) / length(vec);

		vector<TSpace, DIMENSIONS> result;

		details::index_iterator<0, DIMENSIONS - 1>::iterate([&] (size_t i)
		{
			result[i] = vec[i] * inversed_length;
		});

		return result;
	}

	//template <typename TSpace>
	//TSpace dot( const vector<TSpace, 3> & vec1, const vector<TSpace, 3> & vec2 )
	//{
	//	return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
	//}

	template <typename TSpace, std::size_t DIMENSIONS>
	TSpace dot( const vector<TSpace, DIMENSIONS> & vec1, const vector<TSpace, DIMENSIONS> & vec2 )
	{
		TSpace product = TSpace(0);
		details::index_iterator<0, DIMENSIONS - 1>::iterate([&] (size_t i)
		{
			product += vec1[i] * vec2[i];
		});
		return product;
	}
}