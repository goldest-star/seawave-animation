#include "basic_types.hpp"

#include <iostream>
#include <cmath>

namespace vcl
{

bool is_equal(int a, int b) { return a==b; }
bool is_equal(unsigned int a, unsigned int b) { return a==b; }
bool is_equal(size_t a, size_t b) { return a==b; }
bool is_equal(float a, float b)
{
    return std::abs(a-b) <= 1e-6f;
}
bool is_equal(double a, double b)
{
    return std::abs(a-b) <= 1e-8f;
}



constexpr size_t size(int ) {return 1;}
constexpr size_t size(unsigned int ) {return 1;}
constexpr size_t size(unsigned long ) {return 1;}
constexpr size_t size(float ) {return 1;}
constexpr size_t size(double ) {return 1;}

}
