#pragma once

#include "third_party/lodepng/lodepng.h"

#include "vcl/opengl/texture/image/image.hpp"

#include <string>

namespace vcl
{


image_raw image_load_png(const std::string& filename, image_color_type color_type = image_color_type::rgba);

void image_save_png(const std::string& filename, const image_raw& im);

}
