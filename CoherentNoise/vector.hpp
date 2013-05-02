#pragma once

#include "helper.hpp"

#include <cstddef>

namespace noise
{

	// Represents N-dimensional vector in space of TSpace.
	template <typename TSpace, std::size_t DIMENSIONS>
	class vector
	{
	public:

		enum { dimensions = DIMENSIONS };

		vector()
		{
			
		}

		template <typename TOther>
		vector(const vector<TOther, DIMENSIONS> & other )
		{
			for (std::size_t i = 0; i < DIMENSIONS; i++)
			{
				m_elements[i] = TSpace(other[i]);
			}
		}

		template <typename TFirstValue, typename ... TValues>
		vector(const TFirstValue & first_value, const TValues & ... values)
		{
			static_assert(sizeof ... (values) == DIMENSIONS - 1, "Number of arguments does not match dimensionality of the vector!");

			setValues(m_elements, first_value, values ...);
		}

		template <typename ... TValues>
		vector(const vector<TSpace, DIMENSIONS - sizeof ... (TValues)> & vec, const TValues & ... values)
		{
			static_assert(sizeof ... (values) < DIMENSIONS, "Too much arguments!");

			for (std::size_t i = 0; i < DIMENSIONS - sizeof ... (values); i++)
			{
				m_elements[i] = vec[i];
			}

			setValues(m_elements + DIMENSIONS - sizeof ... (values), values ...);
		}

		const TSpace & operator[](std::size_t index) const
		{
			return m_elements[index];
		}

		TSpace & operator[](std::size_t index)
		{
			return m_elements[index];
		}

		template <typename TNewSpace>
		vector<TNewSpace, DIMENSIONS> cast()
		{
			return vector<TNewSpace, DIMENSIONS>(*this);
		}

	private:
		TSpace m_elements[DIMENSIONS];
	};

}