#pragma once

#include "vcl/wrapper/glad/glad.hpp"
#include "../image/image.hpp"



namespace vcl
{


void update_texture_gpu(GLuint texture_id, buffer2D<vec3> const& im);

GLuint create_texture_gpu(buffer2D<vec3> const& im, GLint wrap_s=GL_CLAMP_TO_EDGE, GLint wrap_t=GL_CLAMP_TO_EDGE);
GLuint create_texture_gpu(image_raw const& im, GLint wrap_s=GL_CLAMP_TO_EDGE, GLint wrap_t=GL_CLAMP_TO_EDGE);
GLuint create_texture_gpu(std::vector<unsigned char> const& data, GLsizei width, GLsizei height, GLint wrap_s=GL_CLAMP_TO_EDGE, GLint wrap_t=GL_CLAMP_TO_EDGE);

}
