#include "mesh_drawable_gpu_data.hpp"

#include "vcl/opengl/opengl.hpp"

namespace vcl
{

mesh_drawable_gpu_data::mesh_drawable_gpu_data()
    :vao(0), number_triangles(0), vbo_index(0), vbo_position(0), vbo_normal(0), vbo_color(0), vbo_texture_uv(0)
{}

mesh_drawable_gpu_data::mesh_drawable_gpu_data(const mesh &mesh_cpu_arg)
{
    // Doesn't assign anything if there is no position
    if(mesh_cpu_arg.position.size()==0)
        return;

    // temp copy of the mesh to fill all empty fields
    mesh mesh_cpu = mesh_cpu_arg;
    mesh_cpu.fill_empty_fields();

    // Fill VBO for position
    glGenBuffers(1, &vbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(mesh_cpu.position.size()*sizeof(GLfloat)*3), &mesh_cpu.position[0], GL_DYNAMIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Fill VBO for normal
    glGenBuffers(1, &vbo_normal);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(mesh_cpu.normal.size()*sizeof(GLfloat)*3), &mesh_cpu.normal[0], GL_DYNAMIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Fill VBO for color
    glGenBuffers(1, &vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(mesh_cpu.color.size()*sizeof(GLfloat)*4), &mesh_cpu.color[0], GL_DYNAMIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Fill VBO for texture uv
    glGenBuffers(1, &vbo_texture_uv);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texture_uv);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(mesh_cpu.texture_uv.size()*sizeof(GLfloat)*2), &mesh_cpu.texture_uv[0], GL_DYNAMIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Fill VBO for index
    glGenBuffers(1, &vbo_index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(mesh_cpu.connectivity.size()*sizeof(GLuint)*3), &mesh_cpu.connectivity[0], GL_DYNAMIC_DRAW );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    number_triangles = static_cast<unsigned int>(mesh_cpu.connectivity.size());

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    // position at layout 0
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    // normals at layout 1
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    // colors at layout 2
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, 0, nullptr );

    // texture uv at layout 3
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texture_uv);
    glEnableVertexAttribArray( 3 );
    glVertexAttribPointer( 3, 2, GL_FLOAT, GL_FALSE, 0, nullptr );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void mesh_drawable_gpu_data::clear()
{
    glDeleteBuffers(1,&vbo_position);
    glDeleteBuffers(1,&vbo_normal);
    glDeleteBuffers(1,&vbo_color);
    glDeleteBuffers(1,&vbo_texture_uv);
    glDeleteBuffers(1,&vbo_index);
}

void mesh_drawable_gpu_data::update_position(const buffer<vec3>& new_position)
{
    glBindBuffer(GL_ARRAY_BUFFER,vbo_position);
    assert(glIsBuffer(vbo_position));

    glBufferSubData(GL_ARRAY_BUFFER,0,GLsizeiptr(new_position.size()*sizeof(float)*3),&new_position[0]);
}

void mesh_drawable_gpu_data::update_normal(const buffer<vec3>& new_normal)
{
    glBindBuffer(GL_ARRAY_BUFFER,vbo_normal);
    assert(glIsBuffer(vbo_normal));

    glBufferSubData(GL_ARRAY_BUFFER,0,GLsizeiptr(new_normal.size()*sizeof(float)*3),&new_normal[0]);
}

void draw(const mesh_drawable_gpu_data& gpu_data )
{
    // Doesn't draw if the structure hasn't been initialized
    if(gpu_data.number_triangles==0 && gpu_data.vao==0)
        return ;

    if(gpu_data.number_triangles==0) {
        std::cout<<"Warning, try to draw data with 0 triangles"<<std::endl;
        return ;
    }
    assert(glIsVertexArray(gpu_data.vao));
    assert(glIsBuffer(gpu_data.vbo_index));

    glBindVertexArray(gpu_data.vao); opengl_debug();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_data.vbo_index); opengl_debug();
    glDrawElements(GL_TRIANGLES, GLsizei(gpu_data.number_triangles*3), GL_UNSIGNED_INT, nullptr); opengl_debug();
    glBindVertexArray(0);
}



}
