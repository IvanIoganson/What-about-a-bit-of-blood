#pragma once

class timer 
{
private:
    float start, time, speed, prevtime;
public:
    timer(void);
    float GetTime( void );
    void SetSpeed(float new_speed);
};