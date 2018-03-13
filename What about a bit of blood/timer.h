#pragma once

class timer 
{
private:
    const float start;
    float speed;
    mutable float time, prevtime;
public:
    timer(void);
    float GetTime( void ) const;
    void SetSpeed(float new_speed);
};