#include "picking_primitives.hpp"
#include "vcl/math/math.hpp"

namespace vcl
{

picking_info ray_intersect_sphere(const ray& r, const vec3& center, float radius)
{
    picking_info pick;

    const vec3 d = r.p-center;
    const float b = dot(r.u,d);
    const float c = dot(d,d)-radius*radius;

    const float delta = b*b-c;
    if(delta >= 0)
    {
        const float t0 = -b - std::sqrt(delta);
        const float t1 = -b + std::sqrt(delta);

        const float t = t0>0? t0 : t1;

        if(t>0){
            pick.picking_valid = true;
            pick.intersection = r.p + t*r.u;
            pick.normal = normalize(pick.intersection - center);
        }
    }

    return pick;
}

picking_info ray_intersect_plane(const ray& r, const vec3& n, const vec3& p0)
{
    picking_info pick;


    const float t = - dot(r.p-p0,n)/dot(r.u,n);
    if(t>0)
    {
        pick.picking_valid=true;
        pick.intersection = r.p + t*r.u;
        pick.normal= n;
    }

    return pick;
}

}
