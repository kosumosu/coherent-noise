#pragma once

#include "vector.hpp"

#include <limits>

namespace noise
{
	template <typename TSpace, std::size_t DIMENSIONS>
	TSpace length2( const vector<TSpace, DIMENSIONS> & vec )
	{
		TSpace squared_length = TSpace(0);
		for (std::size_t i = 0; i < DIMENSIONS; i++)
		{
			squared_length += vec[i] * vec[i];
		}
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

		for (std::size_t i = 0; i < DIMENSIONS; i++)
		{
			result[i] = vec[i] * inversed_length;
		}

		return result;
	}



	template <typename TSpace, std::size_t DIMENSIONS>
	TSpace dot( const vector<TSpace, DIMENSIONS> & vec1, const vector<TSpace, DIMENSIONS> & vec2 )
	{
		TSpace product = TSpace(0);
		for (size_t i = 0; i < DIMENSIONS; i++)
		{
			product += vec1[i] * vec2[i];
		}
		return product;
	}
}