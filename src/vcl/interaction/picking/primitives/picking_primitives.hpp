#pragma once

#include "../ray/picking_ray.hpp"
#include "../info/picking_info.hpp"

namespace vcl
{

picking_info ray_intersect_sphere(const ray& r, const vec3& center, float radius);
picking_info ray_intersect_plane(const ray& r, const vec3& n, const vec3& p0);

}
