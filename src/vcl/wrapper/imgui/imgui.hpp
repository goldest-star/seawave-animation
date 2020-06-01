#pragma once

#include "third_party/imgui/imgui.h"
#include "third_party/imgui/imgui_impl_glfw.h"
#include "third_party/imgui/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>


namespace vcl
{

void imgui_init(GLFWwindow* window);

void imgui_create_frame();
void imgui_render_frame(GLFWwindow* window);
void imgui_cleanup();
}
