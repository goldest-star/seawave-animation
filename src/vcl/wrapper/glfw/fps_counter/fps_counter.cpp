#include "fps_counter.hpp"

#include <iostream>

namespace vcl
{

glfw_fps_counter::glfw_fps_counter()
    :frame_count(0),time_previous(0)
{
    time_previous = glfwGetTime();
}

bool glfw_fps_counter::update()
{
    ++frame_count;
    const double time_current = glfwGetTime();
    if( time_current-time_previous > time_update )
        return true;
    else
        return false;
}

void glfw_fps_counter::reset()
{
    time_previous = glfwGetTime();
    frame_count = 0;
}

int glfw_fps_counter::fps() const
{
    const double time_current = glfwGetTime();
    const int fps_number = static_cast<int>(frame_count / (time_current-time_previous));
    return fps_number;
}






}
