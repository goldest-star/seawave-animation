#include "vec2.hpp"

#include <iostream>
#include <cassert>

namespace vcl {

vec2::buffer_stack()
    :x(0),y(0)
{}

vec2::buffer_stack(float x_arg, float y_arg)
    :x(x_arg),y(y_arg)
{}

const float& vec2::operator[](std::size_t index) const
{
    switch(index) {
    case 0:
        return x;
    case 1:
        return y;
    default:
        std::cerr<<"Error: Try to access vec2["<<index<<"]"<<std::endl;
        assert(false);
    }
	abort();
}
float& vec2::operator[](std::size_t index)
{
    switch(index) {
    case 0:
        return x;
    case 1:
        return y;
    default:
        std::cerr<<"Error: Try to access vec2["<<index<<"]"<<std::endl;
        assert(false);
    }
	abort();
}

size_t vec2::size() const
{
    return 2;
}


const float& vec2::operator()(std::size_t index) const
{
    return (*this)[index];
}
float& vec2::operator()(std::size_t index)
{
    return (*this)[index];
}

float const& vec2::at(std::size_t index) const
{
    return (*this)[index];
}
float& vec2::at(std::size_t index)
{
    return (*this)[index];
}


float* vec2::begin() { return &x; }
float* vec2::end() { return &y+1; }
float const* vec2::begin() const { return &x; }
float const* vec2::end() const { return &y+1; }
float const* vec2::cbegin() const { return &x; }
float const* vec2::cend() const { return &y+1; }



}
