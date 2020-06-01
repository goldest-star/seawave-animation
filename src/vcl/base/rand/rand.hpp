#pragma once

#include <random>

namespace vcl
{

/** Generate a random number between value_min and value_max
 * \ingroup math
*/
float rand_interval(const float value_min=0.0f, const float value_max=1.0f);

}
