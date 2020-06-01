#pragma once

#include "vcl/containers/buffer/buffer.hpp"
#include "vcl/math/vec/vec.hpp"

namespace vcl
{

/** \name Linspace
 * Create an equaly-space vector of N-values between two bounds. Syntax inspired from Matlab. */
///@{
/** \relates buffer \ingroup math */
buffer<float> linspace(float x_min, float x_max, size_t N);
/** \relates buffer2D */
buffer2D<vec2> linspace(vec2 const& p_min, vec2 const& p_max, size_t2 N);
/** \relates buffer3D */
buffer3D<vec3> linspace(vec3 const& p_min, vec3 const& p_max, size_t3 N);
///@}

}
