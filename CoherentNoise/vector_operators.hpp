#pragma once

namespace noise
{
	template <typename TSpace, typename TScalar, std::size_t DIMENSIONS>
	vector<TSpace, DIMENSIONS> operator *( const vector<TSpace, DIMENSIONS> & vec, const TScalar & scalar)
	{
		vector<TSpace, DIMENSIONS> result;
		iterate<0, DIMENSIONS - 1>([&] (size_t i) { result[i] = vec[i] * scalar; });
		return result;
	}

	template <typename TSpace, typename TScalar, std::size_t DIMENSIONS>
	vector<TSpace, DIMENSIONS> operator +( const vector<TSpace, DIMENSIONS> & vec1, const vector<TSpace, DIMENSIONS> & vec2 )
	{
		vector<TSpace, DIMENSIONS> result;
		iterate<0, DIMENSIONS - 1>([&] (size_t i) { result[i] = vec1[i] + vec2[i]; });
		return result;
	}

	template <typename TSpace, std::size_t DIMENSIONS>
	vector<TSpace, DIMENSIONS> operator -( const vector<TSpace, DIMENSIONS> & vec1, const vector<TSpace, DIMENSIONS> & vec2 )
	{
		vector<TSpace, DIMENSIONS> result;
		iterate<0, DIMENSIONS - 1>([&] (size_t i) { result[i] = vec1[i] - vec2[i]; });
		return result;
	}

	template <typename TSpace, typename TScalar, std::size_t DIMENSIONS>
	vector<TSpace, DIMENSIONS> operator -( const vector<TSpace, DIMENSIONS> & vec )
	{
		vector<TSpace, DIMENSIONS> result;
		iterate<0, DIMENSIONS - 1>([&] (size_t i) { result[i] = -vec[i]; });
		return result;
	}
}