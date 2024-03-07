#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <limits.h>

class Random {
public:
    #define RAND_MAX INT_MAX

    static void srand(unsigned int seed);
    static int rand();
    static double random(double min, double max);
};

#endif // RANDOM_HPP
