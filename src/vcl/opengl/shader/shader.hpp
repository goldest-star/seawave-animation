#pragma once

#include "vcl/wrapper/glad/glad.hpp"

#include <string>

namespace vcl
{

/** Compile an individual shader provided as a string.
 * Check that the compilation succeed. */
GLuint compile_shader(const std::string& shader_str, const GLenum shader_type);


/** Compile vertex and fragment shaders provided from their file paths, link them, and return a shader program.
 * Check that link operation succeed. */
GLuint create_shader_program(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

GLuint create_shader_program(const std::string& vertex_shader_path, const std::string& geometry_shader_path, const std::string& fragment_shader_path);

}
