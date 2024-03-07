#include <math/random.hpp>

unsigned int rand_seed = 1;

void Random::srand(unsigned int seed) {
    rand_seed = seed;
}

int Random::rand() {
    // Linear congruential generator parameters (Microsoft Visual Studio)
    const unsigned int a = 214013;
    const unsigned int c = 2531011;
    const unsigned int m = 4294967296 - 1; // 2^32

    rand_seed = (a * rand_seed + c) % m;
    return rand_seed >> 16; // Extract bits 16-31 as the pseudo-random number
}

double Random::random(double min, double max) {
    // Generate a random integer between min and max
    int random_int = rand() % (int)(max - min + 1) + min;
    
    // Generate a random number between 0 and 1 by scaling the random integer
    double scale = (double)random_int / RAND_MAX;
    
    // Scale and shift the random number to fit within the specified range
    return min + scale * (max - min);
}
