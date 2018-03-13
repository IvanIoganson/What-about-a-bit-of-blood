#include <ctime>
#include "timer.h"


timer::timer(void) : start(clock()), time(0), speed(1), prevtime(start)
{
}

float timer::GetTime(void) const
{
    int ttt = clock();
    time += (ttt - prevtime) * speed;
    prevtime = ttt;
    return time / CLOCKS_PER_SEC;
}

void timer::SetSpeed(float new_speed)
{
    int ttt = clock();
    time += (ttt - prevtime) * speed;
    prevtime = ttt;
    speed = new_speed;
}