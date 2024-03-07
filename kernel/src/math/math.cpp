#include <math/math.hpp>
#include <math/random.hpp>
#include <limits>

namespace Math {
    int abs(int x) {
        return x < 0 ? -x : x;
    }

    double fabs(double x) {
        return x < 0 ? -x : x;
    }

    double min(double a, double b) {
        return a < b ? a : b;
    }

    double max(double a, double b) {
        return a > b ? a : b;
    }

    double pow(double base, double exponent) {
        if (exponent == 0) return 1;
        if (exponent == 1) return base;

        bool negativeExponent = false;
        if (exponent < 0) {
            negativeExponent = true;
            exponent = -exponent;
        }

        double result = 1;
        while (exponent > 0) {
            result *= base;
            exponent--;
        }

        return negativeExponent ? 1 / result : result;
    }

    double sqrt(double x) {
        if (x < 0) return 0; // Undefined for negative numbers
        if (x == 0 || x == 1) return x;

        double precision = 0.00001; // Adjust as needed for precision
        double start = 0;
        double end = x;
        double mid;

        while (end - start > precision) {
            mid = (start + end) / 2;
            if (mid * mid == x) return mid;
            else if (mid * mid < x) start = mid;
            else end = mid;
        }

        return (start + end) / 2;
    }

    double clamp(double value, double min, double max) {
        if (min > max) {
            double temp = min;
            min = max;
            max = temp;
        }
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    double sin(double angle) {
        double result = 0;
        double term = angle;
        double sign = 1;
        for (int i = 1; i <= 10; i += 2) {
            result += sign * term;
            sign = -sign;
            term *= angle * angle / ((i + 1) * (i + 2));
        }
        return result;
    }

    double cos(double angle) {
        double result = 0;
        double term = 1;
        double sign = 1;
        for (int i = 0; i <= 10; i += 2) {
            result += sign * term;
            sign = -sign;
            term *= angle * angle / ((i + 1) * (i + 2));
        }
        return result;
    }

    double tan(double angle) {
        return sin(angle) / cos(angle);
    }

    double exp(double x) {
        double result = 1;
        double term = 1;
        for (int i = 1; i <= 10; ++i) {
            term *= x / i;
            result += term;
        }
        return result;
    }

    double log(double x) {
        if (x <= 0) {
            // Invalid input
            return 0.0;
        }

        double result = 0.0;
        double term = (x - 1) / (x + 1);
        double termSquared = term * term;
        double currentTerm = term;
        for (int n = 1; n <= 100; n += 2) {
            result += currentTerm / n;
            currentTerm *= termSquared;
        }
        return 2 * result;
    }

    double round(double x) {
        return (x >= 0) ? (int)(x + 0.5) : (int)(x - 0.5);
    }

    double ceil(double x) {
        int integerPart = (int)x;
        return (x == integerPart) ? integerPart : (integerPart + 1);
    }

    double floor(double x) {
        int integerPart = (int)x;
        return (x == integerPart) ? integerPart : (integerPart - 1);
    }

    double sinh(double x) {
        double result = x;
        double term = x;
        int n = 1;
        while (fabs(term) > EPSILON) {
            term *= x * x / ((2 * n) * (2 * n + 1));
            result += term;
            n++;
        }
        return result;
    }

    double cosh(double x) {
        double result = 1;
        double term = 1;
        int n = 1;
        while (fabs(term) > EPSILON) {
            term *= x * x / ((2 * n - 1) * (2 * n));
            result += term;
            n++;
        }
        return result;
    }

    double tanh(double x) {
        return sinh(x) / cosh(x);
    }

    double mod(double dividend, double divisor) {
        if (divisor == 0) {
            // Error: Division by zero
            return 0.0;
        }
        return dividend - (divisor * (int)(dividend / divisor));
    }

    double gcd(double a, double b) {
        if (b == 0) {
            return a;
        }
        return gcd(b, mod(a, b));
    }

    double mean(double values[], int count) {
        if (count <= 0) {
            // Error: Empty array
            return 0.0;
        }
        double sum = 0.0;
        for (int i = 0; i < count; ++i) {
            sum += values[i];
        }
        return sum / count;
    }

    double median(double values[], int count) {
        if (count <= 0) {
            // Error: Empty array
            return 0.0;
        }
        // Sort the array (you can use any sorting algorithm)
        for (int i = 0; i < count - 1; ++i) {
            for (int j = 0; j < count - i - 1; ++j) {
                if (values[j] > values[j + 1]) {
                    double temp = values[j];
                    values[j] = values[j + 1];
                    values[j + 1] = temp;
                }
            }
        }
        // If count is odd, return the middle element
        if (count % 2 != 0) {
            return values[count / 2];
        }
        // If count is even, return the average of the two middle elements
        return (values[count / 2 - 1] + values[count / 2]) / 2.0;
    }

    double random(double min, double max) {
        // Generate a random number between min and max
        double scale = Random::rand() / (double)RAND_MAX; // [0, 1.0]
        return min + scale * (max - min);
    }

    // Function to convert degrees to radians
    double degrees_to_radians(double degrees) {
        return degrees * (PI / 180.0);
    }

    // Function to convert radians to degrees
    double radians_to_degrees(double radians) {
        return radians * (180.0 / PI);
    }

    // Function to calculate factorial of a non-negative integer
    double factorial(double n) {
        if (n < 0) {
            return -1; // Error: factorial of negative number is undefined
        }
        if (n == 0) {
            return 1; // factorial of 0 is defined as 1
        }
        double result = 1;
        for (int i = 1; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

    // Function to check if a number is prime
    bool is_prime(double n) {
        if (n <= 1) {
            return false; // 0 and 1 are not prime numbers
        }
        if (n <= 3) {
            return true; // 2 and 3 are prime numbers
        }
        if (fmod(n, 2) == 0 || fmod(n, 3) == 0) {
            return false; // multiples of 2 and 3 are not prime
        }
        // Check divisibility by numbers of the form 6k ± 1 up to sqrt(n)
        for (int i = 5; i * i <= n; i += 6) {
            if (fmod(n, i) == 0 || fmod(n, i + 2) == 0) {
                return false; // n is divisible by i or i + 2
            }
        }
        return true; // n is prime
    }

    double fmod(double x, double y) {
        if (y == 0) {
            // Division by zero, undefined behavior
            return 0.0;
        }
        double quotient = x / y;
        double whole = (int)quotient;
        return x - whole * y;
    }
}

unsigned int Math::log10(unsigned long long n) {
    unsigned int count = 0;
    while (n >= 10) {
        n /= 10;
        ++count;
    }
    return count;
}

double Math::log10(double x) 
{
    // Check for non-positive arguments
    if (x <= 0) {
        return 0;
    }
        
    return Math::log(x) / Math::log(10.0); // Calculate log base 10 using the change of base formula
}

bool Math::isnan(double x) {
    return x != x;
}

double Math::frexp(double x, int* exp) {
        if (x == 0) {
            if (exp != nullptr)
                *exp = 0;
            return 0;
        }

        int exponent = 0;
        double fraction = Math::frexp(Math::abs(x), &exponent);
        if (exp != nullptr)
            *exp = exponent;
        return fraction * (x < 0 ? -1 : 1);
}

bool Math::isinf(double x)
{
    return x == std::numeric_limits<double>::infinity() || x == -std::numeric_limits<double>::infinity();
}
