#include "vec3.hpp"

#include <iostream>
#include "vcl/base/base.hpp"

namespace vcl {

vec3::buffer_stack()
    :x(0),y(0),z(0)
{}

vec3::buffer_stack(float x_arg, float y_arg, float z_arg)
    :x(x_arg),y(y_arg),z(z_arg)
{}

const float& vec3::operator[](std::size_t index) const
{
    switch(index) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        error_vcl("Try to access vec3["+std::to_string(index)+"]");
    }

}
float& vec3::operator[](std::size_t index)
{
    switch(index) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        error_vcl("Try to access vec3["+std::to_string(index)+"]");
    }

}

size_t vec3::size() const
{
    return 3;
}


const float& vec3::operator()(std::size_t index) const
{
    return (*this)[index];
}
float& vec3::operator()(std::size_t index)
{
    return (*this)[index];
}
float const& vec3::at(std::size_t index) const
{
    return (*this)[index];
}
float& vec3::at(std::size_t index)
{
    return (*this)[index];
}


float* vec3::begin()
{
    return &x;
}
float* vec3::end()
{
    return &z+1;
}
float const* vec3::begin() const
{
    return &x;
}
float const* vec3::end() const
{
    return &z+1;
}
float const* vec3::cbegin() const
{
    return &x;
}
float const* vec3::cend() const
{
    return &z+1;
}

vec3 cross(const vec3& a,const vec3& b)
{
    return    { a.y*b.z-a.z*b.y,
                a.z*b.x-a.x*b.z,
                a.x*b.y-a.y*b.x };
}

}
