#pragma once

#include "vcl/math/vec/vec3/vec3.hpp"

namespace vcl
{

struct picking_info
{
    picking_info();

    bool picking_valid;
    vec3 intersection;
    vec3 normal;
};

}
