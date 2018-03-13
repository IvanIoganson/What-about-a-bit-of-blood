#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "level.h"

#define MAX_PLAYER_MOVE_SPEED 500.0f
#define MAX_PLAYER_MOVE_SPEED_IN_AIR 20.0f
#define MAX_PLAYER_GRAVITY 1000.0f
#define PLAYER_JUMP 600.0f

class player
{
private:
    friend class level;
    texture skin;
    void MoveOnGround(float prev_t);
    void MoveInAir(float prev_t);
public:
    bool is_on_ground;
    int go_x;
    enum {
        ON_GROUND, JUMP, FALL
    } jump_state;
    glm::vec2 g, v, pos;
    timer time;
    level *lvl;
    player(level *nlvl);
    glm::vec2 GetTexCoord(void) const;
    void Response(void);
    void Draw(void) const;
};

extern player *A;