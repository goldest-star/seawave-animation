#pragma once

#include "../camera/camera.hpp"
#include <GLFW/glfw3.h>


namespace vcl
{

class camera_control_glfw
{
public:

    void update_mouse_move(camera_scene& camera, GLFWwindow* window, float x1, float y1);
    void update_mouse_click(camera_scene& camera, GLFWwindow* window,  int button, int action, int mods);

    bool update = true;

private:

    /** previous mouse position (x-coordinate) */
    float x0 = 0.0f;
    /** previous mouse position (y-coordinate) */
    float y0 = 0.0f;



};

}
