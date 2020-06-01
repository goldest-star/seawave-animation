#include "curve_dynamic_drawable.hpp"

namespace vcl
{


curve_dynamic_drawable::curve_dynamic_drawable(size_t max_size_arg)
    :curve_drawable(),position_stored(),max_size(max_size_arg),need_update(true),first_time(true)
{
}

void curve_dynamic_drawable::clear()
{
    position_stored.clear();
    need_update = true;
}

void curve_dynamic_drawable::add_point(const vec3& p)
{
    need_update = true;
    if( position_stored.size()<max_size )
        position_stored.push_back(p);
    else
    {
        for(size_t k=0; k<max_size-1; ++k)
            position_stored[k] = position_stored[k+1];
        position_stored[max_size-1] = p;
    }
}

void curve_dynamic_drawable::draw(GLuint shader_arg, const camera_scene& camera)
{
    if( first_time )
    {
        glGenBuffers(1, &data.vbo_position);
        glBindBuffer(GL_ARRAY_BUFFER, data.vbo_position);
        glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(max_size*sizeof(GLfloat)*3), &position_stored[0], GL_DYNAMIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        data.number_elements = static_cast<unsigned int>(position_stored.size());

        glGenVertexArrays(1,&data.vao);
        glBindVertexArray(data.vao);

        // position at layout 0
        glBindBuffer(GL_ARRAY_BUFFER, data.vbo_position);
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        first_time = false;
    }

    if( need_update )
    {
        data.number_elements = static_cast<unsigned int>(position_stored.size());
        data.update_position(position_stored);

        need_update = false;
    }

    vcl::draw(*this, camera, shader_arg);


}

}
