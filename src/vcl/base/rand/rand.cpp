#include "rand.hpp"

namespace vcl
{


static std::default_random_engine generator(0);
static std::uniform_real_distribution<float> distribution(0,1);

float rand_interval(const float value_min, const float value_max)
{
    return distribution(generator)* (value_max-value_min) + value_min;
}

}
