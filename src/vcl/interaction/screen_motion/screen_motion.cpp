#include "screen_motion.hpp"

namespace vcl
{
void screen_motion_structure::clear()
{
    position.clear();
    time.clear();
}

void screen_motion_structure::add(const vec2& position_arg, float time_arg)
{
    position.push_back(position_arg);
    time.push_back(time_arg);

    if(position.size()>N_position_max)
    {
        position.pop_front();
        time.pop_front();
    }
}

vec2 screen_motion_structure::speed_avg() const
{
    if( position.size()<2 )
        return {0,0};

    const vec2& p1 = *position.begin();
    const vec2& p2 = *position.rbegin();

    const float t1 = *time.begin();
    const float t2 = *time.rbegin();

    if( ! (t2>t1+1e-5f) )
        return {0,0};


    const vec2 s = (p2-p1)/(t2-t1);
    return s;
}
}
