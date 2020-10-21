#include "perlin.hpp"

PerlinNoise::PerlinNoise(unsigned long int seed) {
    // Generate a permutation
    permutation.resize(256);
    std::iota(permutation.begin(), permutation.end(), 0);
    std::default_random_engine rng{seed};
    std::shuffle(permutation.begin(), permutation.end(), rng);
    permutation.insert(permutation.end(), permutation.begin(), permutation.end());
}

PerlinNoise::~PerlinNoise() {

}

double PerlinNoise::noise(double x, double y, double z) {
    int X = (int)floor(x) & 255,
        Y = (int)floor(y) & 255,
        Z = (int)floor(z) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    double u = fade(x),
        v = fade(y),
        w = fade(z);
    int A = permutation[X] + Y, AA = permutation[A]+Z, AB = permutation[A+1]+Z,
        B = permutation[X+1]+Y, BA = permutation[B]+Z, BB = permutation[B+1]+Z;

    double result = lerp(w, lerp(v, lerp(u, grad(permutation[AA  ], x, y, z),
										 grad(permutation[BA  ], x-1, y, z)),
								 lerp(u, grad(permutation[AB  ], x, y-1, z),
									  grad(permutation[BB  ], x-1, y-1, z))),
						 lerp(v, lerp(u, grad(permutation[AA+1], x, y, z-1 ),
									  grad(permutation[BA+1], x-1, y, z-1 )),
							  lerp(u, grad(permutation[AB+1], x, y-1, z-1 ),
								   grad(permutation[BB+1], x-1, y-1, z-1 ))));
    return result;
}

double PerlinNoise::fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    double u = h < 8 ? x : y,
        v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
