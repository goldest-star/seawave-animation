#pragma once

namespace vcl
{

class time_period
{
public:
    time_period();

    bool update();
    float time_step;

private:
    float t0;
};

}
