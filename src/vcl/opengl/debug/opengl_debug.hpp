#pragma once

#include "vcl/wrapper/glad/glad.hpp"

#include <string>

#ifdef __linux__
#define P_FUNCTION __PRETTY_FUNCTION__
#else
#define P_FUNCTION __FUNCTION__
#endif

#define opengl_debug() vcl::check_opengl_error(__FILE__,P_FUNCTION,__LINE__)

namespace vcl
{

void opengl_debug_print_version();
void check_opengl_error(const std::string& file, const std::string& function, int line);

}
