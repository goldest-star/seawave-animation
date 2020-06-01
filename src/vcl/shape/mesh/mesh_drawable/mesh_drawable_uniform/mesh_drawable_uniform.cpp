#include "mesh_drawable_uniform.hpp"


namespace vcl
{

shading_mesh::shading_mesh(float ambiant_arg, float diffuse_arg, float specular_arg, int specular_exponent_arg)
    :ambiant(ambiant_arg), diffuse(diffuse_arg), specular(specular_arg), specular_exponent(specular_exponent_arg)
{}

mesh_drawable_uniform::mesh_drawable_uniform()
    :transform(), color({1,1,1}), shading(), color_alpha(1.0f)
{}



}
