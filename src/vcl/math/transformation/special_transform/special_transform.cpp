#include "special_transform.hpp"
#include "vcl/math/math.hpp"

namespace vcl
{

mat3 rotation_from_axis_angle_mat3(const vec3& axis, float angle)
{
    const vec3 u = normalize(axis);
    const float x = u.x;
    const float y = u.y;
    const float z = u.z;
    const float c = std::cos(angle);
    const float s = std::sin(angle);

    return mat3 {c+x*x*(1-c)  , x*y*(1-c)-z*s, x*z*(1-c)+y*s,
                 y*x*(1-c)+z*s, c+y*y*(1-c)  , y*z*(1-c)-x*s,
                 z*x*(1-c)-y*s, z*y*(1-c)+x*s, c+z*z*(1-c)   };
}

mat3 rotation_between_vector_mat3(const vec3& a, const vec3& b)
{
    const vec3 u0 = normalize(a);
    const vec3 u1 = normalize(b);

    if( norm(u0-u1)<1e-4f )
        return mat3::identity();
    if( norm(u0+u1)<1e-4f )
        return -mat3::identity();

    const float d = dot(u0,u1);
    const float angle = std::acos( d );

    vec3 axis = normalize(cross(u0,u1));

    return rotation_from_axis_angle_mat3(axis,angle);
}

}
