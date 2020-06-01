#pragma once

#include <vector>

#include "vcl/containers/buffer/buffer2D/buffer2D.hpp"
#include "vcl/math/vec/vec3/vec3.hpp"

namespace vcl
{


enum class image_color_type {rgb, rgba};

struct image_raw
{
    image_raw();
    image_raw(unsigned int width_arg, unsigned int height_arg, image_color_type color_type_arg, std::vector<unsigned char> const& data_arg);

    unsigned int width;
    unsigned int height;
    image_color_type color_type;
    std::vector<unsigned char> data;

    buffer2D<vec3> to_buffer_rgb() const;
};



}
