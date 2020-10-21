#ifndef PERLIN_HPP
#define PERLIN_HPP
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

// This is a translated and improved version of the Perlin noise java implementation (https://mrl.nyu.edu/~perlin/noise/)
// Credits go to Ken Perlin, not me

class PerlinNoise {
public:
	PerlinNoise(unsigned long int seed);
	~PerlinNoise();
	double noise(double x, double y, double z = 0.0);

private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

	std::vector<int> permutation; // Maybe an array would work well too
};


#endif // PERLIN_HPP
