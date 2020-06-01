#include "uniform.hpp"

namespace vcl
{

void uniform(GLuint shader, const std::string& name, const int value)
{
    const GLint location = glGetUniformLocation(shader, name.c_str());
    glUniform1i(location, value);
}

void uniform(GLuint shader, const std::string& name, float value)
{
    const GLint location = glGetUniformLocation(shader, name.c_str());
    glUniform1f(location, value);
}


void uniform(GLuint shader, const std::string& name, const vec3& value)
{
    const GLint location = glGetUniformLocation(shader, name.c_str());
    glUniform3f(location, value.x,value.y, value.z);
}

void uniform(GLuint shader, const std::string& name, const vec4& value)
{
    const GLint location = glGetUniformLocation(shader, name.c_str());
    glUniform4f(location, value.x,value.y, value.z, value.w);
}

void uniform(GLuint shader, const std::string& name, float x, float y, float z)
{
    const GLint location = glGetUniformLocation(shader, name.c_str());
    glUniform3f(location, x, y, z);
}

void uniform(GLuint shader, const std::string& name, float x, float y, float z, float w)
{
    const GLint location = glGetUniformLocation(shader, name.c_str());
    glUniform4f(location, x, y, z, w);
}

void uniform(GLuint shader, const std::string& name, const mat4& m)
{
    const GLint location = glGetUniformLocation(shader, name.c_str());
    const float* ptr = &m[0];
    glUniformMatrix4fv(location, 1, GL_TRUE, ptr);
}

void uniform(GLuint shader, const std::string& name, const mat3& m)
{
    const GLint location = glGetUniformLocation(shader, name.c_str());
    const float* ptr = &m[0];
    glUniformMatrix3fv(location, 1, GL_TRUE, ptr);
}



}
