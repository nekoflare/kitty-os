//
// Created by Piotr on 23.03.2024.
//

#ifndef KITTY_OS_CPP_MATH_H
#define KITTY_OS_CPP_MATH_H

#include <limits>

namespace ktl
{
    // Define INFINITY and -INFINITY for float
    constexpr float INFINITY = std::numeric_limits<float>::infinity();
    constexpr float NEG_INFINITY = -std::numeric_limits<float>::infinity();

    // Define INFINITY and -INFINITY for double
    constexpr double D_INFINITY = std::numeric_limits<double>::infinity();
    constexpr double D_NEG_INFINITY = -std::numeric_limits<double>::infinity();

    // Define INFINITY and -INFINITY for long double
    constexpr long double LD_INFINITY = std::numeric_limits<long double>::infinity();
    constexpr long double LD_NEG_INFINITY = -std::numeric_limits<long double>::infinity();

    template <typename T>
    T abs(T value) {
        return (value < 0) ? -value : value;
    }

    template<typename T>
    bool isinf(T x) {
        // Check if the value is positive or negative infinity
        return x == INFINITY || x == -INFINITY;
    }

    template<typename T>
    bool isnan(T x) {
        // Check if the value is NaN
        return x != x;
    }

    template<typename T>
    T log10(T x) {
        // to implement: static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

        T result = 0;
        if (x <= 0)
            return std::numeric_limits<T>::quiet_NaN();

        while (x >= 10) {
            result += 1;
            x /= 10;
        }

        while (x < 1) {
            result -= 1;
            x *= 10;
        }

        T power_of_10 = 10;
        while (power_of_10 < x) {
            power_of_10 *= 10;
            result += 1;
        }

        return result;
    }

    template<typename T>
    T pow(T base, int exponent) {
        // to implement: static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");

        if (exponent == 0) {
            return 1;
        } else if (exponent < 0) {
            T result = 1;
            for (int i = 0; i > exponent; --i) {
                result /= base;
            }
            return result;
        } else {
            T result = 1;
            for (int i = 0; i < exponent; ++i) {
                result *= base;
            }
            return result;
        }
    }
}

#endif //KITTY_OS_CPP_MATH_H
