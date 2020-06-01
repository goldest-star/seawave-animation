#include "curve_gpu.hpp"

#include "vcl/base/base.hpp"
#include "vcl/opengl/debug/opengl_debug.hpp"

namespace vcl
{

curve_gpu::curve_gpu()
    :vao(0),vbo_position(0),number_elements(0)
{}

curve_gpu::curve_gpu(const std::vector<vec3>& position)
    :vao(0),vbo_position(0),number_elements(0)
{
    // Fill VBO for position
    glGenBuffers(1, &vbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(position.size()*sizeof(GLfloat)*3), &position[0], GL_DYNAMIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    number_elements = static_cast<unsigned int>(position.size());

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    // position at layout 0
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void curve_gpu::update_position(const std::vector<vec3>& new_position)
{
    glBindBuffer(GL_ARRAY_BUFFER,vbo_position);
    assert_vcl(glIsBuffer(vbo_position), "Incorrect vbo");

    glBufferSubData(GL_ARRAY_BUFFER,0,GLsizeiptr(new_position.size()*sizeof(float)*3),&new_position[0]);
}

void draw(const curve_gpu& curve)
{
    glBindVertexArray(curve.vao); opengl_debug();
    glDrawArrays(GL_LINE_STRIP, 0, GLsizei(curve.number_elements)); opengl_debug();
    glBindVertexArray(0);
}



}
