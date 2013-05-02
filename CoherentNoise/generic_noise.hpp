#pragma once

#include "vector.hpp"

namespace noise
{
	template <std::size_t DIMENSIONS, typename TSpace>
	class generic_noise
	{
	public:
		virtual void initialize(unsigned int seed) = 0;
		virtual TSpace evaluate(const vector<TSpace, DIMENSIONS> & point) const = 0;
	};
}