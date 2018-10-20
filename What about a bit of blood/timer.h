#pragma once

class timer 
{
private:
    const float start;
    float speed;
    float time, prevtime;
public:
    timer(int cur_t = 0, float init_speed = 1.f);
    void SynchronizeTimer(int cur_frame_t);
    float GetTime(void) const;
    void SetSpeed(float new_speed);
};