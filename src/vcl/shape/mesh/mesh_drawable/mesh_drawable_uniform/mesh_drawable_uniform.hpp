#pragma once


#include "vcl/math/math.hpp"

namespace vcl
{

struct shading_mesh {
    shading_mesh(float ambiant=0.2f, float diffuse=0.8f, float specular=0.5f, int specular_exponent=128);

    float ambiant;
    float diffuse;
    float specular;
    int specular_exponent;
};

struct mesh_drawable_uniform {

    mesh_drawable_uniform();


    affine_transform transform;
    vec3 color;
    shading_mesh shading;
    float color_alpha;

};

}
