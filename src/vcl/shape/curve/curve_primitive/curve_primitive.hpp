#pragma once

#include "vcl/math/vec/vec3/vec3.hpp"
#include <vector>

namespace vcl {

std::vector<vcl::vec3> curve_primitve_circle(int N_sample=40, float radius=1.0f, const vcl::vec3& center={0,0,0}, const vcl::vec3& normal={0,0,1});

}
