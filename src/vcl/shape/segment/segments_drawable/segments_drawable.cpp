#include "segments_drawable.hpp"

#include "vcl/opengl/opengl.hpp"

namespace vcl
{

segments_drawable::segments_drawable()
    :uniform(),data(),shader(0)
{}

segments_drawable::segments_drawable(const segments_gpu& data_arg)
    :uniform(),data(data_arg),shader(0)
{}
segments_drawable::segments_drawable(const std::vector<vec3>& data_arg)
    :uniform(),data(segments_gpu(data_arg)),shader(0)
{}

void draw(const segments_drawable& shape, const camera_scene& camera)
{
    draw(shape, camera, shape.shader);
}
void draw(const segments_drawable& shape, const camera_scene& camera, GLuint shader)
{
    // Check shader and only switch if necessary
    if( glIsProgram(shader)==GL_FALSE ) {
        std::cout<<"Try to display a mesh with invalid shader ("<<shader<<"): skip display"<<std::endl;
        return ;
    }

                                                                          opengl_debug();
    GLint current_shader = 0;                                             opengl_debug();
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);                   opengl_debug();

    if(shader!=GLuint(current_shader))
        glUseProgram(shader);                                             opengl_debug();

    uniform(shader, "rotation", shape.uniform.transform.rotation);        opengl_debug();
    uniform(shader, "translation", shape.uniform.transform.translation);  opengl_debug();
    uniform(shader, "scaling", shape.uniform.transform.scaling);          opengl_debug();

    uniform(shader, "color", shape.uniform.color);                        opengl_debug();


    uniform(shader,"perspective",camera.perspective.matrix());            opengl_debug();
    uniform(shader,"view",camera.view_matrix());                          opengl_debug();

    vcl::draw(shape.data);                                                opengl_debug();
}

}
