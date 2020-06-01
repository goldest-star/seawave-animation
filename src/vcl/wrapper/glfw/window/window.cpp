#include "window.hpp"

#include <iostream>

namespace vcl
{


static std::string glfw_error_string(int error)
{
    switch(error)
    {
    case GLFW_NOT_INITIALIZED:
        return "GLFW_NOT_INITIALIZED";
    case GLFW_INVALID_ENUM:
        return "GLFW_INVALID_ENUM";
    case GLFW_INVALID_VALUE:
        return "GLFW_INVALID_VALUE";
    case GLFW_API_UNAVAILABLE:
        return "GLFW_API_UNAVAILABLE";
    case GLFW_VERSION_UNAVAILABLE:
        return "GLFW_VERSION_UNAVAILABLE";
    case GLFW_FORMAT_UNAVAILABLE:
        return "GLFW_FORMAT_UNAVAILABLE";
    case GLFW_PLATFORM_ERROR:
        return "GLFW_PLATFORM_ERROR";
    default:
        return "UNKNOWN ERROR (Shouldn't happen)";
    }

    return "IMPOSSIBLE ERROR! (Should never happen)";
}

static void glfw_error_callback(int error, const char* description)
{
    std::cerr<<"Received GLFW error"<<std::endl;
    std::cerr<<"\t Error "<<glfw_error_string(error)<<" ("<<error<<")"<<std::endl;
    std::cerr<<"\t Description - "<<description<<std::endl;
}

void glfw_init()
{
    glfwSetErrorCallback(glfw_error_callback);

    const int glfw_init_value = glfwInit();
    if( glfw_init_value != 1 ) {
        std::cerr<<"Failed to Init GLFW"<<std::endl;
        exit(1);
    }

}

GLFWwindow* glfw_create_window(int width, int height, const std::string& title, int opengl_version_major, int opengl_version_minor, GLFWmonitor* monitor, GLFWwindow* share)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_FLOATING, 0);


    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), monitor, share);
    if( window==nullptr ) {
        std::cerr<<"Failed to create GLFW Window"<<std::endl;
        std::cerr<<"\t Possible error cause: Incompatible OpenGL version (requesting OpenGL "<<opengl_version_major<<"."<<opengl_version_minor<<")"<<std::endl;
        std::cerr<<"\t Check your current system OpenGL version (ex. glxinfo)"<<std::endl;
        exit(1);
    }

    glfwMakeContextCurrent(window);

    return window;

}

void glad_init()
{
    const int glad_init_value = gladLoadGL();
    if( glad_init_value == 0 ) {
        std::cerr<<"Failed to Init GLAD"<<std::endl;
        exit(1);
    }
}

}
