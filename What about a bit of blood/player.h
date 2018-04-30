#pragma once

#include "level.h"
#include "mesh_2d.h"

#define MAX_PLAYER_MOVE_SPEED_ON_GROUND 500.0f
#define MAX_PLAYER_MOVE_SPEED_IN_AIR 600.0f
#define MAX_PLAYER_GRAVITY 4000.0f
#define PLAYER_JUMP_SPEED 700.0f
#define PLAYER_NORMAL_ACCELERATION_X_ON_GROUND 10.f
#define PLAYER_NORMAL_ACCELERATION_X_IN_AIR 4.f

#define MAX_MOVE_UP_TIME 0.2f

class player
{
private:
    texture skin;
    mesh_2d *mesh;
    void MoveOnGround(float prev_t);
    void MoveInAir(float prev_t);
public:
    bool is_on_ground;
    double go_x;
    enum {
        ON_GROUND, JUMP, FALL, MOVE_UP
    } jump_state;
    enum {
        STOP, LEFT, RIGHT
    } moving;
    glm::vec2 g, v, pos;
    timer time;
    level *lvl;
    player(level *nlvl);
    glm::vec2 GetTexCoord(void) const;
    void Response(void);
    void Draw(void) const;

    friend class level;
};

extern player *A;
