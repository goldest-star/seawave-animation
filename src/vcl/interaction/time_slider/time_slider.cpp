#include "time_slider.hpp"

#include "vcl/wrapper/glfw/glfw.hpp"

#include <cassert>

namespace vcl
{

timer_interval_reversing::timer_interval_reversing()
    :timer_basic(),t_min(0.0f),t_max(1.0f),forward_direction(true)
{}

float timer_interval_reversing::update()
{
    assert(t_min<t_max);

    if(!running)
        return 0.0f;

    const float time_current = static_cast<float>(glfwGetTime());
    const float dt = scale*(time_current-time_previous);
    assert(dt<t_max-t_min);

    const float direction = forward_direction==true? 1.0f : -1.0f ;
    time_previous = time_current;
    t = t + direction*dt;


    if( t>=t_max )
    {
        t = 2 * t_max-t;
        forward_direction = false;
    }
    else if( t<t_min )
    {
        t = 2 * t_min-t;
        forward_direction = true;
    }

    return dt;
}

timer_interval::timer_interval()
    :timer_basic(),t_min(0.0f),t_max(1.0f)
{}

float timer_interval::update()
{
    assert(t_max>t_min);

    float dt = timer_basic::update();
    if( dt>t_max-t_min )
        dt = 0;

    while( t>=t_max )
        t -= (t_max-t_min);

    assert(t>=t_min && t<t_max);

    return dt;
}

timer_event::timer_event()
    :timer_basic(),event(false),periodic_event_time_step(1.0f),time_since_last_event(0)
{}

float timer_event::update()
{
    const float dt = timer_basic::update();

    time_since_last_event += dt;
    if( time_since_last_event > periodic_event_time_step )
    {
        event = true;
        time_since_last_event = 0;
    }
    else
        event = false;

    return dt;
}



timer_basic::timer_basic()
    :t(0),scale(1.0f),running(true),time_previous(static_cast<float>(glfwGetTime()))
{}

float timer_basic::update()
{
    if(!running)
        return 0.0f;

    const float time_current = static_cast<float>(glfwGetTime());
    const float dt = scale*(time_current-time_previous);

    time_previous = time_current;
    t += dt;

    return dt;
}

void timer_basic::start()
{
    running = true;
    time_previous = static_cast<float>(glfwGetTime());
}
void timer_basic::stop()
{
    running = false;
}





time_slider::time_slider()
    :t_min(0.0f),t_max(1.0f),t(0.0f), reversing(false), time_scale(1.0f), running(true), reversed_direction(false), t_absolute_stored( static_cast<float>(glfwGetTime()) )
{
}

time_slider::time_slider(float t_min_arg, float t_max_arg, bool running_arg, bool reversing_arg, float time_scale_arg)
    :t_min(t_min_arg),t_max(t_max_arg),t(t_min_arg), reversing(reversing_arg), time_scale(time_scale_arg), running(running_arg), reversed_direction(false), t_absolute_stored( static_cast<float>(glfwGetTime()) )
{
}



void time_slider::update()
{
    if( running==true )
    {
        const float time_absolute = static_cast<float>(glfwGetTime());
        const float dt = time_absolute - t_absolute_stored;
        t_absolute_stored = time_absolute;

        if(!reversing)
        {
            t += time_scale*dt;
            if( t>t_max )
                t -= t_max-t_min;
        }
        else
        {
            if(!reversed_direction)
                t += time_scale*dt;
            else
                t -= time_scale*dt;

            if( t>=t_max )
            {
                t = t_max;
                reversed_direction = true;
            }
            if( t<=t_min )
            {
                t = t_min;
                reversed_direction = false;
            }
        }

    }
}

void time_slider::stop()
{
    running = false;
}

void time_slider::run()
{
    running = true;
    t_absolute_stored = static_cast<float>(glfwGetTime());
}





}
