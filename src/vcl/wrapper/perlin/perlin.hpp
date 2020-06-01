#pragma once

#include "third_party/simplexnoise/simplexnoise1234.hpp"

namespace  vcl {

float perlin(float x, int octave=5, float persistency=0.3f, float frequency_gain=2.0f);
float perlin(float x, float y, int octave=5, float persistency=0.3f, float frequency_gain=2.0f);
float perlin(float x, float y, float z, int octave=5, float persistency=0.3f, float frequency_gain=2.0f);

}
