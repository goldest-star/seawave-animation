#pragma once

#include "../segments_gpu/segments_gpu.hpp"
#include "vcl/interaction/camera/camera.hpp"
#include "segments_drawable_uniform/segments_drawable_uniform.hpp"

namespace vcl
{

struct segments_drawable
{
public:
    segments_drawable();
    segments_drawable(const segments_gpu& data);
    segments_drawable(const std::vector<vec3>& data);

    segments_drawable_uniform uniform;
    segments_gpu data;
    GLuint shader;
};

void draw(const segments_drawable& shape, const camera_scene& camera);
void draw(const segments_drawable& shape, const camera_scene& camera, GLuint shader);

}
