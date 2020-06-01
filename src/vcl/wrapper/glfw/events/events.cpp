#include "events.hpp"

namespace vcl
{

bool glfw_mouse_pressed_left(GLFWwindow* window)
{
    return (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT )==GLFW_PRESS);
}
bool glfw_mouse_released_left(GLFWwindow* window)
{
    return (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT )==GLFW_RELEASE);
}
bool glfw_mouse_pressed_right(GLFWwindow* window)
{
    return (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT )==GLFW_PRESS);
}
bool glfw_mouse_released_right(GLFWwindow* window)
{
    return (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT )==GLFW_RELEASE);
}

bool glfw_key_shift_pressed(GLFWwindow* window)
{
    return (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT));
}
bool glfw_key_ctrl_pressed(GLFWwindow* window)
{
    return (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL));
}


vec2 glfw_cursor_coordinates_window(GLFWwindow* window)
{
    // Window size
    int w=0, h=0;
    glfwGetWindowSize(window, &w, &h);

    // Current cursor position
    double xpos=0, ypos=0;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Convert pixel coordinates to relative screen coordinates between [-1,1]
    const float x = 2*float(xpos)/float(w)-1;
    const float y = 1-2*float(ypos)/float(h);

    return {x,y};
}

}
