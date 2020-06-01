#include "segment_drawable_immediate_mode.hpp"

#include "vcl/opengl/opengl.hpp"

namespace vcl
{

segment_drawable_immediate_mode::segment_drawable_immediate_mode()
    :uniform_parameter({{0,0,0},{1,0,0},{1,0,0}}),data_gpu(),initialized(false)
{}

void segment_drawable_immediate_mode::init()
{
    data_gpu = segments_gpu( {{0,0,0},{1,0,0}} );
    initialized = true;
}



void segment_drawable_immediate_mode::draw(GLuint shader, const camera_scene& camera)
{
    if(initialized==false)
    {
        std::cerr<<" > Error segment_drawable_immediate_mode is not initialized"<<std::endl;
        std::cerr<<" > Make sure you call init() function before calling draw()"<<std::endl;
        exit(1);
    }

                                                                    opengl_debug();
    GLint current_shader = 0;                                       opengl_debug();
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);             opengl_debug();

    if(shader!=GLuint(current_shader))
        glUseProgram(shader);                                       opengl_debug();

    uniform(shader, "color", uniform_parameter.color);              opengl_debug();
    uniform(shader, "p1", uniform_parameter.p1);                    opengl_debug();
    uniform(shader, "p2", uniform_parameter.p2);                    opengl_debug();

    uniform(shader,"perspective",camera.perspective.matrix());      opengl_debug();
    uniform(shader,"view",camera.view_matrix());                    opengl_debug();

    vcl::draw(data_gpu);                                            opengl_debug();
}

}
