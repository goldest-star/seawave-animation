#include "hierarchy_mesh_drawable_node.hpp"

namespace vcl {

hierarchy_mesh_drawable_node::hierarchy_mesh_drawable_node()
    :element(), name("undefined"), name_parent("global_frame"), transform(), global_transform()
{}

hierarchy_mesh_drawable_node::hierarchy_mesh_drawable_node(const mesh_drawable& element_arg,
                                                           const std::string& name_arg,
                                                           const std::string& name_parent_arg,
                                                           const affine_transform& transform_arg)
    :element(element_arg), name(name_arg), name_parent(name_parent_arg), transform(transform_arg), global_transform()
{}



hierarchy_mesh_drawable_node::hierarchy_mesh_drawable_node(const mesh_drawable& element_arg,
                             const std::string& name_arg,
                             const std::string& name_parent_arg,
                             const vec3& translation)
    :element(element_arg), name(name_arg), name_parent(name_parent_arg), transform(), global_transform()
{
    transform.translation = translation;
}


}
