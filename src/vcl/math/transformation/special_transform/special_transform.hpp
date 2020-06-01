#pragma once

#include "../../vec/vec.hpp"
#include "../../mat/mat.hpp"


namespace vcl
{

/** \ingroup math
 * @{
 */



/** Return matrix corresponding to a 3D rotation parameterized by an axis and an angle
    ex. rotation_from_axis_angle_mat3({1,0,0}, 3.14f/2) // rotation of pi/2 around the x-axis
    The axis is normalized in the function.
*/
mat3 rotation_from_axis_angle_mat3(const vec3& axis, float angle);

/** Return a matrix of rotation R such that b = R a
 * a and b are normalized in the function
 * R is defined by:
 *   - the rotation-axis:  n = a x b
 *   - the rotation-angle: theta = acos( a.b )
 * Note that R is generally not the unique possible rotation */
mat3 rotation_between_vector_mat3(const vec3& a, const vec3& b);

/** @} */

}
