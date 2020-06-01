#pragma once


#include "vcl/math/math.hpp"

namespace vcl
{

struct segments_drawable_uniform {

    segments_drawable_uniform();

    affine_transform transform;
    vec3 color;
};

}
