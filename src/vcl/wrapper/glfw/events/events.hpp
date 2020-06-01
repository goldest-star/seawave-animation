#pragma once

#include "vcl/wrapper/glad/glad.hpp"
#include "vcl/wrapper/glfw/glfw.hpp"

#include "vcl/math/vec/vec2/vec2.hpp"

namespace vcl
{

bool glfw_mouse_pressed_left(GLFWwindow* window);
bool glfw_mouse_released_left(GLFWwindow* window);

bool glfw_mouse_pressed_right(GLFWwindow* window);
bool glfw_mouse_released_right(GLFWwindow* window);

bool glfw_key_shift_pressed(GLFWwindow* window);
bool glfw_key_ctrl_pressed(GLFWwindow* window);

/** Coordinates of the cursor position on screen normalized in [0,1] */
vec2 glfw_cursor_coordinates_window(GLFWwindow* window);

}
