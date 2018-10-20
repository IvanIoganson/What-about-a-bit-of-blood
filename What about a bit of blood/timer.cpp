#include "timer.h"

timer::timer(int cur_t, float init_speed) : start(cur_t), time(0), speed(init_speed), prevtime(start)
{
}

void timer::SynchronizeTimer(int cur_frame_t)
{
    time += (cur_frame_t - prevtime) * speed;
    prevtime = cur_frame_t;
}

float timer::GetTime(void) const
{
    return time / 1000;
}

void timer::SetSpeed(float new_speed)
{
    speed = new_speed;
}