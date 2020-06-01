#pragma once

#include "vcl/wrapper/glad/glad.hpp"
#include "vcl/math/vec/vec3/vec3.hpp"
#include <vector>

namespace vcl
{
struct curve_gpu
{
    curve_gpu();
    curve_gpu(const std::vector<vec3>& position);

    void update_position(const std::vector<vec3>& new_position);

    GLuint vao;
    GLuint vbo_position;
    unsigned int number_elements;
};

void draw(const curve_gpu& curve);
}
