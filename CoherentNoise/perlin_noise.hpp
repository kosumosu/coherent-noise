#pragma once

#include "generic_noise.hpp"
#include "vector_generation.hpp"
#include "vector_operators.hpp"

namespace noise
{
	template <std::size_t DIMENSIONS, typename TSpace>
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
			traverse_info_t traverse_info;
			for (size_t i = 0; i < dimensions; i++)
			{
				auto node_coord = space_int_t(point[i]);
				auto local_coord = point[i] - node_coord;

				traverse_info.min_corner_coords[i] = node_coord;
				//traverse_info.max_corner_coords[i] = node_coord + space_int_t(1);
				traverse_info.local_coords[i] = local_coord;
				//traverse_info.inverted_local_coords[i] = local_coord - space_t(1);
				traverse_info.s_curve[i] = s_curve(local_coord);
			}

			auto sampled_value = evaluator<0>::evaluate_imp(m_perturbations, m_gradients, table_size_t(0), traverse_info);

			return sampled_value;
		}

	private:
		typedef unsigned int table_size_t;
		typedef int space_int_t;
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
			//grid_vector max_corner_coords;

			noise_vector local_coords;
			//noise_vector inverted_local_coords;
			space_t s_curve[dimensions];
		};

		struct level_info_t
		{
			space_t local_coord;
			space_int_t node_coord;
		};

		template <size_t DIMENSION>
		class evaluator
		{
		public:
			enum { dimension = DIMENSION };
			template <typename ... TLevels>
			static space_t evaluate_imp(const table_size_t perturbations[table_size], const noise_vector gradients[table_size], table_size_t perturbation,
				const traverse_info_t & traverse_info, const TLevels & ... level_infos)
			{
				level_info_t left_level_info = { traverse_info.local_coords[dimension], traverse_info.min_corner_coords[dimension] };
				level_info_t right_level_info = { traverse_info.local_coords[dimension] - space_t(1), traverse_info.min_corner_coords[dimension] + space_int_t(1) };

				auto left_perturbation = perturbations[(perturbation + left_level_info.node_coord) & table_mask];
				auto right_perturbation = perturbations[(perturbation + right_level_info.node_coord) & table_mask];

				auto left_value = evaluator<dimension + 1>::evaluate_imp(perturbations, gradients, left_perturbation, traverse_info, level_infos ..., left_level_info);
				auto right_value = evaluator<dimension + 1>::evaluate_imp(perturbations, gradients, right_perturbation, traverse_info, level_infos ..., right_level_info);

				auto interpolated_value = lerp(left_value, right_value, traverse_info.s_curve[dimension]);
				return interpolated_value;
			}

		private:
			static space_t lerp(const space_t & left_value, const space_t & right_value, const space_t & fraction)
			{
				return left_value + fraction * (right_value - left_value);
			}
		};

		template <>
		class evaluator<dimensions>
		{
		public:
			enum { dimension = 0 };
			template <typename ... TLevels>
			static space_t evaluate_imp(const table_size_t perturbations[table_size], const noise_vector gradients[table_size], table_size_t perturbation,
				const traverse_info_t & traverse_info, const TLevels & ... level_infos)
			{
				const noise_vector local_position(level_infos.local_coord ...);

				auto gradient = gradients[perturbation];

				auto dot_product = noise::dot(local_position, gradient);
				return dot_product;
			}
		};
	};
}