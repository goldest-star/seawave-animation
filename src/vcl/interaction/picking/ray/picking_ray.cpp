#include "picking_ray.hpp"
#include "vcl/math/math.hpp"

namespace vcl
{

ray::ray()
    :p(),u({1,0,0})
{}

ray::ray(const vec3& p_arg, const vec3& u_arg)
    :p(p_arg),u(u_arg)
{}

ray picking_ray(const camera_scene& camera, const vec2& cursor)
{
    ray r;
    r.p = camera.camera_position();

    vec4 dir_screen = {cursor.x,cursor.y,-1.0f,1.0f};
    mat4 Proj_inv = camera.perspective.matrix_inverse();
    mat4 View_inv = camera.camera_matrix();

    vec4 dir_eye = Proj_inv * dir_screen;
    vec4 dir = View_inv * vec4(dir_eye.x,dir_eye.y,-1.0f,0.0f);

    r.u = normalize(vec3(dir[0],dir[1],dir[2]));

    return r;
}

}
