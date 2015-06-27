#pragma once

#include <random>

#include "vector_operations.hpp"

namespace noise
{
	// Generates normalized random vector with spherical probability distribution.
	template <typename TSpace, std::size_t DIMENSIONS, typename TRandomEngine>
	vector<TSpace, DIMENSIONS> random_spherical(TRandomEngine & random_engine)
	{
		std::normal_distribution<TSpace> distribution;
		vector<TSpace, DIMENSIONS> random_vector;

		iterate<0, DIMENSIONS - 1>([&] (size_t i) { random_vector[i] = distribution(random_engine); });

		// There's a possibility of zero-length vector!
		return DIMENSIONS > 1 ? noise::normalize(random_vector) : random_vector;
	}

}