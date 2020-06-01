#pragma once

#include "../../math/vec/vec3/vec3.hpp"
#include "../../math/mat/mat4/mat4.hpp"
#include "../../math/mat/mat3/mat3.hpp"

namespace vcl
{

struct perspective_structure
{
    float angle_of_view;
    float image_aspect;
    float z_near;
    float z_far;

    perspective_structure();
    perspective_structure(float angle_of_view, float image_aspect, float z_near, float z_far);

    mat4 matrix() const;
    mat4 matrix_inverse() const;
};

enum camera_control_type {camera_control_trackball, camera_control_spherical_coordinates};

/** Structure handling a camera.
    The camera handle internally
    - a scale parameter (zoom in/out) - float
    - a translation (camera position) - vec3
    - an orientation - mat3 */
struct camera_scene
{
    /** Base distance between camera and (0,0,0) along z-direction */
    static constexpr float scale0 = 3.0f;

    float scale      = scale0;
    vec3 translation = {};
    mat3 orientation = {};
    perspective_structure perspective = {};

    /** Compute the view matrix to be multiplied to vertices */
    mat4 view_matrix() const;
    mat4 camera_matrix() const;
    /** Return the corresponding center of camera */
    vec3 camera_position() const;

    camera_control_type camera_type = camera_control_trackball;
    vec2 spherical_coordinates = {0,0};

    // Apply transformation to camera
    void apply_translation_in_screen_plane(float tr_x, float tr_y);
    void apply_translation_orthogonal_to_screen_plane(float tr);
    void apply_rotation(float p0x, float p0y, float p1x, float p1y);
    void apply_scaling(float s);
};


/** Compute a 4x4 perspective matrix */
mat4 perspective_matrix(float angle_of_view, float image_aspect, float z_near, float z_far);

}
