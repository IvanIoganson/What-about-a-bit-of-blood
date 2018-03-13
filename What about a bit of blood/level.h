#pragma once

#include <glm\matrix.hpp>

#include "timer.h"
#include "map.h"
#include "texture.h"

using namespace std;

class level
{
private:
    map map;
    timer time;
    unsigned int fbo;
    texture fbo_texture;
    glm::mat4 camera;
    plane p;

    void DrawFBO(void);
    void ResponseCamera(void);
public:
    level(string map_name);
    ~level();
    void Draw(void);

    friend class player;
};

extern level *lvl;
extern int WidthScreen, HeightScreen;
extern double ratio;
