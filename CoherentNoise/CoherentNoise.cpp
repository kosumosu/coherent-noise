// CoherentNoise.cpp : Defines the entry point for the console application.
//

#include "vector.hpp"
#include "vector_operations.hpp"
#include "perlin_noise.hpp"
#include "vector_operators.hpp"
#include "HRTimer.h"

#include <png.hpp>
#include <iostream>
#include <string>
#include <iomanip>

typedef float space_t;

int main(int argc, char * argv[])
{
	if (argc < 4)
	{
		std::cout << "Syntax : CoherentNoise <image width> <image height> <output filename>" << std::endl;
		return 0;
	}
	const int width = std::stoi(argv[1]);
	const int height = std::stoi(argv[2]);
	const std::string output_filename(argv[3]);

	png::image<png::rgb_pixel> image(width, height);

	noise::perlin_noise<3, space_t> noise_gen;
	noise_gen.initialize(1234);

	HRTimer timer;

	timer.Restart();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			noise::vector<space_t, 3> vec(x * space_t(0.125), y * space_t(0.125), space_t(0));
			auto result = noise_gen.evaluate(vec) * space_t(0.5) + space_t(0.5);

			auto image_value = static_cast<png::byte>(noise::saturate(result * space_t(255), space_t(0), space_t(255)));
			image[y][x].red = image_value;
			image[y][x].green = image_value;
			image[y][x].blue = image_value;
		}
	}

	auto time = timer.Sample();

	std::cout << "Completed in " << std::fixed << std::setprecision(6) << std::showpoint << time << " sec" << std::endl;
	
	image.write(output_filename);

	return 0;
}

