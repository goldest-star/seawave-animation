#include "perlin.hpp"

namespace  vcl {

float perlin(float x, int octave, float persistency, float frequency_gain)
{
    float value = 0.0f;
    float a = 1.0f; // current magnitude
    float f = 1.0f; // current frequency
    for(int k=0;k<octave;k++)
    {
        const float n = static_cast<float>(snoise1(x*f));
        value += a*(0.5f+0.5f*n);
        f *= frequency_gain;
        a *= persistency;
    }
    return value;
}

float perlin(float x, float y, int octave, float persistency, float frequency_gain)
{
    float value = 0.0f;
    float a = 1.0f; // current magnitude
    float f = 1.0f; // current frequency
    for(int k=0;k<octave;k++)
    {
        const float n = static_cast<float>(snoise2(x*f, y*f));
        value += a*(0.5f+0.5f*n );
        f *= frequency_gain;
        a *= persistency;
    }
    return value;
}
float perlin(float x, float y, float z, int octave, float persistency, float frequency_gain)
{
    float value = 0.0f;
    float a = 1.0f; // current magnitude
    float f = 1.0f; // current frequency
    for(int k=0;k<octave;k++)
    {
        const float n = static_cast<float>(snoise3(x*f, y*f, z*f));
        value += a*(0.5f+0.5f*n);
        f *= frequency_gain;
        a *= persistency;
    }
    return value;
}

}
