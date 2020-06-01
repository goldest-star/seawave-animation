#include "time_period.hpp"

#include "vcl/wrapper/glfw/glfw.hpp"

namespace vcl
{

time_period::time_period()
    :time_step(1.0f), t0(static_cast<float>(glfwGetTime()))
{}

bool time_period::update()
{
    const float t1 = static_cast<float>(glfwGetTime());
    if( t1-t0>time_step )
    {
        t0=t1;
        return true;
    }
    else
        return false;
}

}
