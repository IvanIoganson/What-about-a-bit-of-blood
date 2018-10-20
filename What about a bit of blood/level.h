#pragma once

#include <glm\matrix.hpp>
#include <list>

#include "timer.h"
#include "map.h"
#include "texture.h"

using namespace std;

class level
{
private:
    list<map *> maps;
    unsigned int fbo;
    texture fbo_texture;
    glm::mat4 view;
    glm::mat4 projection;

    void DrawFBO(void) const;
    void ResponseCamera(void);
public:
    plane *p;
    timer time;
    glm::mat4 fbo_projection;

    level(string map_name);
    ~level();
    void Draw(void);

    friend class player;
    friend class object;
};

extern level *lvl;
extern int WidthScreen, HeightScreen;
extern double ratio;
extern int MapWidth, MapHeight;
