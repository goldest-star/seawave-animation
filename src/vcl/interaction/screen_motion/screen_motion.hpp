#pragma once

#include "vcl/math/vec/vec2/vec2.hpp"

#include <list>

namespace vcl
{

struct screen_motion_structure
{
    void clear();
    void add(const vec2& position_arg, float time_arg);
    vec2 speed_avg() const;

    size_t N_position_max = 10;
    std::list<vec2> position = {};
    std::list<float> time    = {};
};


}
