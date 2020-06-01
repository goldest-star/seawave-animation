#pragma once

#include "vcl/shape/mesh/mesh_drawable/mesh_drawable.hpp"
#include "vcl/shape/segment/segment_drawable_immediate_mode/segment_drawable_immediate_mode.hpp"



namespace vcl{

struct hierarchy_mesh_drawable;


/** Helper class associated to hierarchy_mesh_drawable
 * Ease the process of displaying the skeleton of a hierarchy_mesh_drawable (ex. debugging purpose)
 * The class must be initialized using the appropriate shader once, and then the draw function can be called on a hierarchy_mesh_drawable */
class hierarchy_mesh_drawable_display_skeleton
{
public:
    hierarchy_mesh_drawable_display_skeleton();

    // Initialize once the class with the appropriate shaders
    void init(GLuint shader_segment_immediate_mode, GLuint shader_mesh);

    // Display the hierarchy_mesh_drawable sent as parameter
    void draw(hierarchy_mesh_drawable const& hierarchy, const camera_scene& camera);

    // Size of the display frames
    float frame_scaling = 0.08f; // adapt this frame_scaling if necessary


private:

    bool initialized;

    GLuint shader_segment_immediate_mode;
    GLuint shader_mesh;

    vcl::segment_drawable_immediate_mode segment_drawer;
    vcl::mesh_drawable frame_visual;
};

}
