#pragma once

namespace vcl
{



class timer_basic
{
public:
    timer_basic();

    float update();
    void start();
    void stop();

    float t;
    float scale;

protected:
    bool running;
    float time_previous;
};

class timer_event : public timer_basic
{
public:
    timer_event();

    float update();

    bool event;
    float periodic_event_time_step;

private:
    float time_since_last_event;
};


class timer_interval : public timer_basic
{
public:
    timer_interval();

    float update();

    float t_min;
    float t_max;
};

class timer_interval_reversing : public timer_basic
{
public:
    timer_interval_reversing();

    float update();

    float t_min;
    float t_max;
private:
    bool forward_direction;
};



class time_slider
{
public:

    time_slider();
    time_slider(float t_min, float t_max, bool running, bool reversing, float time_scale);

    void update();

    void stop();
    void run();

    float t_min;
    float t_max;
    float t;
    bool reversing;
    float time_scale;

private:
    bool running;
    bool reversed_direction;
    float t_absolute_stored;
};

}
