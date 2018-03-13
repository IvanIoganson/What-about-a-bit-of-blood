#pragma once

#include "timer.h"
#include "vec.h"

class object
{
private:
    timer timer;
    vec<> pos;
public:
    virtual void Draw(void) {};
    virtual void Response(void) {};

};