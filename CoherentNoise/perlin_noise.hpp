#pragma once

#include "generic_noise.hpp"
#include "vector_generation.hpp"
#include "vector_operators.hpp"

namespace noise
{
	template <std::size_t DIMENSIONS, typename TSpace, typename TIntSpace>
	class perlin_noise : public generic_noise<DIMENSIONS, TSpace>
	{
	public:
		typedef TSpace space_t;
		enum { dimensions = DIMENSIONS };
		typedef vector<space_t, dimensions> noise_vector;

		virtual void initialize(unsigned int seed) override
		{
			generate_perturbations_table(seed);
			generate_gradients_table();
		}

		virtual space_t evaluate(const noise_vector & point) const override
		{
			perlin_evaluator evaluator(m_perturbations, m_gradients, point);
			auto sampled_value = evaluator.evaluate();
			return sampled_value;
		}

	private:
		typedef unsigned int table_size_t;
		typedef TIntSpace space_int_t;
		typedef vector<space_int_t, dimensions> grid_vector;

		enum { table_size = 256, table_mask = table_size - 1 };

		table_size_t m_perturbations[table_size];
		noise_vector m_gradients[table_size]; // can be made static


		void generate_perturbations_table(unsigned int seed)
		{
			std::uniform_int_distribution<table_size_t> distribution(0, table_size - 1);
			std::default_random_engine random_engine;
			random_engine.seed(seed);

			for (table_size_t i = 0; i < table_size; i++)
			{
				m_perturbations[i] = i;
			}

			for (table_size_t i = 0; i < table_size; i++)
			{
				auto random_index = distribution(random_engine);
				std::swap(m_perturbations[i], m_perturbations[random_index]);
			}
		}

		void generate_gradients_table()
		{
			std::default_random_engine random_engine;

			for (table_size_t i = 0; i < table_size; i++)
			{
				m_gradients[i] = noise::random_spherical<space_t, dimensions>(random_engine);
			}
		}

		static space_t s_curve(const space_t & value)
		{
			return value * value * (space_t(3) - space_t(2) * value);
			//return value * value * value * (space_t(10) + value * (space_t(-15) + value * space_t(6)));
		}

		struct traverse_info_t
		{
			grid_vector min_corner_coords;
			noise_vector local_coords;
			space_t s_curve[dimensions];
		};

		struct level_info_t
		{
			space_t local_coord;
			space_int_t node_coord;
		};

		template <size_t level>
		struct tag { };

		class perlin_evaluator
		{
		public:

			perlin_evaluator(const table_size_t perturbations[table_size], const noise_vector gradients[table_size], const noise_vector & point)
				: m_perturbations(perturbations)
				, m_gradients(gradients)
			{
				init_dimension(tag<0>(), point);
			}

			space_t evaluate() const
			{
				return evaluate_imp(tag<0>(), table_size_t(0));
			}

		private:
			const table_size_t * m_perturbations;
			const noise_vector * m_gradients;

			grid_vector m_min_corner_coords;
			noise_vector m_local_coords;
			space_t m_s_curve[dimensions];

			template <size_t DIMENSION>
			void init_dimension(tag<DIMENSION>, const noise_vector & point)
			{
				init_dimension_imp<DIMENSION>(point);
				init_dimension(tag<DIMENSION + 1>(), point);
			}

			void init_dimension(tag<dimensions - 1>, const noise_vector & point)
			{
				init_dimension_imp<dimensions - 1>(point);
			}

			template <size_t DIMENSION>
			void init_dimension_imp(const noise_vector & point)
			{
				auto node_coord = space_int_t(noise::fast_floor<space_int_t>(point[DIMENSION]));
				auto local_coord = point[DIMENSION] - node_coord;

				m_min_corner_coords[DIMENSION] = node_coord;
				m_local_coords[DIMENSION] = local_coord;
				m_s_curve[DIMENSION] = s_curve(local_coord);
			}

			template <size_t DIMENSION, typename ... TLevels>
			space_t evaluate_imp(tag<DIMENSION>, table_size_t perturbation, const TLevels & ... level_infos) const
			{
				level_info_t left_level_info = { m_local_coords[DIMENSION], m_min_corner_coords[DIMENSION] };
				level_info_t right_level_info = { m_local_coords[DIMENSION] - space_t(1), m_min_corner_coords[DIMENSION] + space_int_t(1) };

				auto left_perturbation = m_perturbations[(perturbation + left_level_info.node_coord) & table_mask];
				auto right_perturbation = m_perturbations[(perturbation + right_level_info.node_coord) & table_mask];

				auto left_value = evaluate_imp(tag<DIMENSION + 1>(), left_perturbation, level_infos ..., left_level_info);
				auto right_value = evaluate_imp(tag<DIMENSION + 1>(), right_perturbation, level_infos ..., right_level_info);

				auto interpolated_value = lerp(left_value, right_value, m_s_curve[DIMENSION]);
				return interpolated_value;
			}

			static space_t lerp(const space_t & left_value, const space_t & right_value, const space_t & fraction)
			{
				return left_value + fraction * (right_value - left_value);
			}

			template <typename ... TLevels>
			space_t evaluate_imp(tag<dimensions>, table_size_t perturbation, const TLevels & ... level_infos) const
			{
				const noise_vector local_position(level_infos.local_coord ...);

				auto gradient = m_gradients[perturbation];

				auto dot_product = noise::dot(local_position, gradient);
				return dot_product;
			}
		};

	};
}