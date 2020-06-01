#include "shader.hpp"

#include "vcl/base/base.hpp"

#include <vector>
#include <iostream>
#include <cassert>

namespace vcl
{

static void check_compilation(GLuint shader,const std::string& shader_str)
{
    GLint is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);

    // get info on compilation
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<GLchar> infoLog( static_cast<size_t>(maxLength)+1 );
    glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

    if( maxLength >= 1 )
    {
        std::cerr << "[Info from shader compilation]"<< std::endl;
        std::cerr << &infoLog[0] << std::endl;
        std::cerr << "For shader " << shader_str << std::endl;
    }

    if( is_compiled==GL_FALSE )
    {
        std::cerr << "Compilation Failed" <<std::endl;
        glDeleteShader(shader);
        exit(1);
    }
}

static void check_link(GLuint vertex_shader, GLuint fragment_shader, GLuint program)
{
    GLint is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);

    // Get info on Link
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<GLchar> infoLog( static_cast<size_t>(maxLength)+1 );
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    if( maxLength >= 1 )
    {
        std::cerr << "[Info from shader Link]"<< std::endl;
        std::cerr << &infoLog[0] << std::endl;
    }
    if( is_linked==GL_FALSE ) //if failed link
    {
        // clean memory
        glDeleteProgram( program );
        glDeleteShader( vertex_shader);
        glDeleteShader( fragment_shader);

        std::cerr << "Failed to link shader program" << std::endl;
        exit(1);
    }

}

GLuint compile_shader(const std::string& shader_str, const GLenum shader_type)
{
    const GLuint shader = glCreateShader(shader_type); assert( glIsShader(shader) );
    char const* const shader_cstring = shader_str.c_str();
    glShaderSource(shader, 1, &shader_cstring, nullptr);

    // Compile shader
    glCompileShader( shader );

    check_compilation(shader, shader_str);

    return shader;
}


GLuint create_shader_program(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
    const std::string vertex_shader_str   = read_file_text(vertex_shader_path);
    const std::string fragment_shader_str = read_file_text(fragment_shader_path);

    const GLuint vertex_shader   = compile_shader(vertex_shader_str, GL_VERTEX_SHADER);
    const GLuint fragment_shader = compile_shader(fragment_shader_str, GL_FRAGMENT_SHADER);

    assert( glIsShader(vertex_shader) );
    assert( glIsShader(fragment_shader) );

    // Create Program
    const GLuint program = glCreateProgram();
    assert( glIsProgram(program) );

    // Attach Shader to Program
    glAttachShader( program, vertex_shader );
    glAttachShader( program, fragment_shader );

    // Link Program
    glLinkProgram( program );

    check_link(vertex_shader, fragment_shader, program);

    // Shader can be detached.
    glDetachShader( program, vertex_shader);
    glDetachShader( program, fragment_shader);


    return program;
}

GLuint create_shader_program(const std::string& vertex_shader_path, const std::string& geometry_shader_path, const std::string& fragment_shader_path)
{
    const std::string vertex_shader_str   = read_file_text(vertex_shader_path);
    const std::string geometry_shader_str = read_file_text(geometry_shader_path);
    const std::string fragment_shader_str = read_file_text(fragment_shader_path);

    const GLuint vertex_shader   = compile_shader(vertex_shader_str, GL_VERTEX_SHADER);
    const GLuint geometry_shader = compile_shader(geometry_shader_str, GL_GEOMETRY_SHADER);
    const GLuint fragment_shader = compile_shader(fragment_shader_str, GL_FRAGMENT_SHADER);

    assert( glIsShader(vertex_shader) );
    assert( glIsShader(geometry_shader) );
    assert( glIsShader(fragment_shader) );

    // Create Program
    const GLuint program = glCreateProgram();
    assert( glIsProgram(program) );

    // Attach Shader to Program
    glAttachShader( program, vertex_shader );
    glAttachShader( program, geometry_shader );
    glAttachShader( program, fragment_shader );

    // Link Program
    glLinkProgram( program );

    check_link(vertex_shader, fragment_shader, program);

    // Shader can be detached.
    glDetachShader( program, vertex_shader);
    glDetachShader( program, geometry_shader);
    glDetachShader( program, fragment_shader);


    return program;
}

}
