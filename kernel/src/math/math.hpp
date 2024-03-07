#ifndef MATH_HPP
#define MATH_HPP


namespace Math {
    #define EPSILON 0.00000001 // Small value to compare floating point numbers
    #define PI 3.14159265358979323846

    int abs(int x);
    double fabs(double x);
    double min(double a, double b);
    double max(double a, double b);
    double pow(double base, double exponent);
    double sqrt(double x);
    double clamp(double value, double min, double max);
    double sin(double angle);
    double cos(double angle);
    double tan(double angle);
    double exp(double x);
    double log(double x);
    double round(double x);
    double ceil(double x);
    double floor(double x);
    double sinh(double x);
    double cosh(double x);
    double tanh(double x);
    double degrees_to_radians(double degrees);
    double radians_to_degrees(double radians);
    double factorial(double n);
    bool is_prime(double n);
    double fmod(double x, double y);
    unsigned int log10(unsigned long long n);
    double log10(double x);
    bool isnan(double x);
    double frexp(double x, int* exp);
    bool isinf(double v);
}

#endif // MATH_HPP
