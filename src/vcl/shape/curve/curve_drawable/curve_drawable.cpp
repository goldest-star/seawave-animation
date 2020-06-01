#include "curve_drawable.hpp"

#include "vcl/opengl/opengl.hpp"

namespace vcl
{

curve_drawable::curve_drawable()
    :uniform(),data(),shader(0)
{}

curve_drawable::curve_drawable(const curve_gpu& data_arg)
    :uniform(),data(data_arg),shader(0)
{}
curve_drawable::curve_drawable(const std::vector<vec3>& data_arg)
    :uniform(),data(curve_gpu(data_arg)),shader(0)
{}

void draw(const curve_drawable& drawable, const camera_scene& camera)
{
    draw(drawable, camera, drawable.shader);
}

void draw(const curve_drawable& drawable, const camera_scene& camera, GLuint shader)
{

    // If shader is 0, use the current one
    GLint current_shader = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader); opengl_debug();
    if(shader==0) {
        shader = GLuint(current_shader);
    }
    // Check that the shader is a valid one
    if( glIsProgram(shader)==GL_FALSE ) {
        std::cout<<"No valid shader set to display mesh: skip display"<<std::endl;
        return;
    }
    // Switch shader program only if necessary
    if(shader!=GLuint(current_shader))
        glUseProgram(shader); opengl_debug();


    uniform(shader, "rotation", drawable.uniform.transform.rotation);        opengl_debug();
    uniform(shader, "translation", drawable.uniform.transform.translation);  opengl_debug();
    uniform(shader, "color", drawable.uniform.color);                        opengl_debug();
    uniform(shader, "scaling", drawable.uniform.transform.scaling);          opengl_debug();

    uniform(shader,"perspective",camera.perspective.matrix());      opengl_debug();
    uniform(shader,"view",camera.view_matrix());                    opengl_debug();

    vcl::draw(drawable.data);                                                opengl_debug();
}

}
