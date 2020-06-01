#include "hierarchy_mesh_drawable_display_skeleton.hpp"

#include "../hierarchy_mesh_drawable.hpp"

#include "vcl/shape/mesh/mesh.hpp"


#include <set>

namespace vcl {


hierarchy_mesh_drawable_display_skeleton::hierarchy_mesh_drawable_display_skeleton()
    :initialized(false), shader_segment_immediate_mode(0), shader_mesh(0), segment_drawer(), frame_visual()
{}

void hierarchy_mesh_drawable_display_skeleton::init(GLuint shader_segment_immediate_mode_arg,
                                                 GLuint shader_mesh_arg)
{
    assert_vcl(initialized==false, "hierarchy_mesh_drawable_display_debug already initialized");

    shader_segment_immediate_mode = shader_segment_immediate_mode_arg;
    shader_mesh = shader_mesh_arg;

    segment_drawer.init();
    frame_visual = mesh_primitive_frame();
    frame_visual.uniform.transform.scaling = 0.1f;

    initialized = true;

}


void hierarchy_mesh_drawable_display_skeleton::draw(const hierarchy_mesh_drawable& hierarchy, const camera_scene& camera)
{
    assert_vcl(initialized==true, "hierarchy_mesh_drawable_display_debug not initialized");

    const size_t N = hierarchy.elements.size();
    for(size_t k=0; k<N; ++k)
    {
        const hierarchy_mesh_drawable_node& node = hierarchy.elements[k];
        const affine_transform& T = node.global_transform;

        mat3 const R = T.rotation * node.element.uniform.transform.rotation;
        vec3 const p = T.translation + node.element.uniform.transform.translation;

        frame_visual.uniform.transform.scaling = frame_scaling;
        frame_visual.uniform.transform.translation = p;
        frame_visual.uniform.transform.rotation = R;

        // Display the current frame
        vcl::draw(frame_visual, camera, shader_mesh);

        // Display the skeleton between parent-current position
        if( k>0 ){

            // Get parent position
            const hierarchy_mesh_drawable_node& node_parent = hierarchy[node.name_parent];
            const affine_transform& T_parent = node_parent.global_transform;
            vec3 const p_parent = T_parent.translation + node_parent.element.uniform.transform.translation;

            // Display the segment
            segment_drawer.uniform_parameter.p1 = p_parent;
            segment_drawer.uniform_parameter.p2 = p;
            segment_drawer.draw(shader_segment_immediate_mode, camera);
        }


    }
}


}
