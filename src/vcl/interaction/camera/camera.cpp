#include "camera.hpp"

#include "../../math/transformation/transformation.hpp"

namespace vcl
{

perspective_structure::perspective_structure()
    :angle_of_view(50*3.14159f/180), image_aspect(1), z_near(0), z_far(1)
{}

perspective_structure::perspective_structure(float angle_of_view_arg, float image_aspect_arg, float z_near_arg, float z_far_arg)
    :angle_of_view(angle_of_view_arg), image_aspect(image_aspect_arg), z_near(z_near_arg), z_far(z_far_arg)
{}

mat4 perspective_structure::matrix() const
{
    const float fy = 1/std::tan(angle_of_view/2);
    const float fx = fy/image_aspect;
    const float L = z_near-z_far;

    const float C = (z_far+z_near)/L;
    const float D = (2*z_far*z_near)/L;

    return {
        fx,0,0,0,
                0,fy,0,0,
                0,0,C,D,
                0,0,-1,0
    };
}

mat4 perspective_structure::matrix_inverse() const
{
    const float fy = 1/std::tan(angle_of_view/2);
    const float fx = fy/image_aspect;
    const float L = z_near-z_far;

    const float C = (z_far+z_near)/L;
    const float D = (2*z_far*z_near)/L;

    return {
        1/fx,0,0,0,
                0,1/fy,0,0,
                0,0,0,-1,
                0,0,1/D,C/D
    };
}



mat4 camera_scene::view_matrix() const
{
    mat3 R = transpose(orientation);
    vec3 T = vec3{0,0,-scale} + R*translation;

    return    { R(0,0), R(0,1), R(0,2), T.x,
                R(1,0), R(1,1), R(1,2), T.y,
                R(2,0), R(2,1), R(2,2), T.z,
                0  ,    0  ,   0   ,  1 };

}

mat4 camera_scene::camera_matrix() const
{
    mat3 R = orientation;
    vec3 T = R*vec3{0,0,scale} - translation;

    return    { R(0,0), R(0,1), R(0,2), T.x,
                R(1,0), R(1,1), R(1,2), T.y,
                R(2,0), R(2,1), R(2,2), T.z,
                0  ,    0  ,   0   ,  1 };
}


mat4 perspective_matrix(float angle_of_view, float image_aspect, float z_near, float z_far)
{
    const float fy = 1/std::tan(angle_of_view/2);
    const float fx = fy/image_aspect;
    const float L = z_near-z_far;

    const float C = (z_far+z_near)/L;
    const float D = (2*z_far*z_near)/L;

    return {
        fx,0,0,0,
                0,fy,0,0,
                0,0,C,D,
                0,0,-1,0
    };

}



void camera_scene::apply_translation_in_screen_plane(float tr_x, float tr_y)
{
    const float alpha = scale/scale0;
    translation += (alpha+0.5f) * orientation * vec3{tr_x, tr_y, 0.0f};
}
void camera_scene::apply_translation_orthogonal_to_screen_plane(float tr)
{
    const float alpha = scale/scale0;
    translation += (alpha+0.5f) * orientation * vec3{0.0f, 0.0f, tr};
}

static vec3 trackball_projection(float x, float y, float radius=1.0f)
{
    const float d = std::sqrt(x*x + y*y);
    float z = 0;
    if( d<radius/std::sqrt(2.0f) )
        z = std::sqrt(radius*radius-d*d)/radius;
    else
        z = radius * radius / (2 * d);

    return {x, y, z};
}


void camera_scene::apply_rotation(float x0, float y0, float x1, float y1)
{

    if(camera_type == camera_control_spherical_coordinates)
    {
        // Assume z-coordinate is up-direction
        const float dtheta = x1-x0;
        const float dphi = y1-y0;

        spherical_coordinates.x -= dtheta;
        spherical_coordinates.y += dphi;
        const float theta = spherical_coordinates.x;
        const float phi   = spherical_coordinates.y;

        const mat3 Rx = { 1,      0        ,     0           ,
                          0,std::cos(phi), -std::sin(phi),
                          0,std::sin(phi),  std::cos(phi)};

        const mat3 Rz = { std::cos(theta) , -std::sin(theta) , 0,
                          std::sin(theta) ,  std::cos(theta) , 0,
                          0             ,      0         , 1};

        orientation = Rz*Rx;
    }
    else if (camera_type == camera_control_trackball )
    {
        /** Trackball implementation from http://ws.iat.sfu.ca/papers/comparerotation.pdf */
        const float d = std::sqrt( (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) );
        if( d < 1e-6f )
            return;

        const vec3 p0 = trackball_projection(x0, y0);
        const vec3 p1 = trackball_projection(x1, y1);

        const mat3 R = rotation_between_vector_mat3(p0, p1);
        orientation = orientation * transpose(R);
    }

}

void camera_scene::apply_scaling(float s)
{
    scale *= (1.0f+s);
    scale = std::max(scale, 0.01f);
}

vec3 camera_scene::camera_position() const
{
    return orientation*vec3{0,0,scale} - translation;;
}

}
