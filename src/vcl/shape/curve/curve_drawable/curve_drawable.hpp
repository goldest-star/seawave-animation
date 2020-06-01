#pragma once


#include "vcl/interaction/camera/camera.hpp"
#include "curve_drawable_uniform/curve_drawable_uniform.hpp"
#include "../curve_gpu/curve_gpu.hpp"

namespace vcl
{

struct curve_drawable
{
public:
    curve_drawable();
    curve_drawable(const curve_gpu& data);
    curve_drawable(const std::vector<vec3>& data);

    curve_drawable_uniform uniform;
    curve_gpu data;
    GLuint shader;
};

void draw(const curve_drawable& drawable, const camera_scene& camera);
void draw(const curve_drawable& drawable, const camera_scene& camera, GLuint shader);

}
