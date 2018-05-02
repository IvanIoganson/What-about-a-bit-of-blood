#include <iostream>

#include <GL/glew.h> 

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>   
#else
#include <GLUT/GLUT.h>  
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "player.h"

player *A;

void player::MoveOnGround(float prev_t)
{
    glm::vec2 n = cur_surf->GetNormal(pos);
    g = -n;
    
    if (moving == LEFT)
        go_x -= (time.GetTime() - prev_t) * PLAYER_NORMAL_ACCELERATION_X_ON_GROUND,
        go_x = go_x < -1 ? -1 : go_x;
    else if (moving == RIGHT)
        go_x += (time.GetTime() - prev_t) * PLAYER_NORMAL_ACCELERATION_X_ON_GROUND,
        go_x = go_x > 1 ? 1 : go_x;
    else
        go_x = 0;

    if (moving != STOP)
    {
        glm::vec2 start = pos;
        int x = pos.x, y = pos.y;
        glm::vec2 d;
        float len = MAX_PLAYER_MOVE_SPEED_ON_GROUND * (time.GetTime() - prev_t);
        if (len < 1)
            len = 1;
        for (int i = 0; i < len; i++)
        {
            d = glm::vec2(n.y * go_x, -n.x * go_x);
            pos += d;
            if ((int)pos.x != x || (int)pos.y != y)
            {
                /*for (auto i = lvl->maps.begin(); i != lvl->maps.end(); i++)
                    if ((*i)->IsOnMap(pos))
                        if ((*i)->im.GetPixel(pos.x - (*i)->pos.x, pos.y - (*i)->pos.y).a > 0)
                            cur_surf = *i;*/
                if (cur_surf->im.GetPixel(pos.x, pos.y).a == 0)
                {
                    pos = cur_surf->GetCollision(pos, g);
                }
                else if (cur_surf->im.GetNumOfNotCromaPoints(pos) > 8)
                {
                    pos = cur_surf->GetFreePlace(pos, n);
                }
                x = pos.x, y = pos.y;
                n = cur_surf->GetNormal(pos);
                g = -n;
            }
        }
        v = (pos - start) / (time.GetTime() - prev_t);
    }
    else
        v = glm::vec2(0,0);
    if (jump_state == FALL || jump_state == MOVE_UP)
    {
        jump_state = ON_GROUND;
        is_on_ground = true;
    }
    if (jump_state == JUMP)
    {
        n = cur_surf->GetNormal(pos);
        v += n * PLAYER_JUMP_SPEED;
        g = -n;
        is_on_ground = false;
        while (cur_surf->im.GetPixel(pos.x, pos.y).a > 0)
            pos += n;
    }
}

void player::MoveInAir(float prev_t)
{
    static glm::vec2 v_y(0,0);
    glm::vec2 nv, npos;
    static float jump_start;

    if (moving == LEFT)
        go_x -= (time.GetTime() - prev_t) * PLAYER_NORMAL_ACCELERATION_X_IN_AIR,
        go_x = go_x < -1 ? -1 : go_x;
    else if (moving == RIGHT)
        go_x += (time.GetTime() - prev_t) * PLAYER_NORMAL_ACCELERATION_X_IN_AIR,
        go_x = go_x > 1 ? 1 : go_x;

    if (jump_state == JUMP)
        jump_start = time.GetTime(),
        jump_state = MOVE_UP;
    if (jump_state == MOVE_UP) {
        v_y = glm::vec2((g.x * v.x + g.y * v.y) * g.x, (g.x * v.x + g.y * v.y) * g.y);
        if (jump_start + MAX_MOVE_UP_TIME < time.GetTime())
            jump_state = FALL;
    }
    if (jump_state == FALL)
        v_y += g * (time.GetTime() - prev_t) * MAX_PLAYER_GRAVITY;

    v = glm::vec2(-MAX_PLAYER_MOVE_SPEED_IN_AIR * g.y * go_x, MAX_PLAYER_MOVE_SPEED_IN_AIR * g.x * go_x) + v_y;
    npos = pos + v * (time.GetTime() - prev_t);

    glm::vec2 d = glm::normalize(npos - pos);
    while (pos.y >= 0 && pos.y < MapHeight && pos.x >= 0 && pos.x < MapWidth && glm::dot(d, (npos - pos)) > 0)
    {
        if (cur_surf->im.GetPixel(pos.x, pos.y).a > 0) {
            cur_surf = cur_surf;
            break;
        }
        for (auto i = lvl->maps.begin(); i != lvl->maps.end(); i++)
            if ((*i)->IsOnMap(pos))
                if ((*i)->im.GetPixel(pos.x - (*i)->pos.x, pos.y - (*i)->pos.y).a > 0)
                {
                    cur_surf = *i;
                    break;
                }
        pos += d;
    }
}

player::player(level *nlvl): skin(), lvl(nlvl), pos(400,300), g(0, 1), is_on_ground(false),
                             go_x(0), moving(STOP), jump_state(FALL), cur_surf(*nlvl->maps.begin())
{
    image im("player.png");
    skin.BindImg(im);
    float vert[] = {
        0 - (float)skin.GetWidth() / 2, 0                          , 0, 0, 1,
        0 - (float)skin.GetWidth() / 2, 0 + (float)skin.GetHeight(), 0, 0, 0,
        0 + (float)skin.GetWidth() / 2, 0 + (float)skin.GetHeight(), 0, 1, 0,
        0 + (float)skin.GetWidth() / 2, 0                          , 0, 1, 1
    };
    GLuint ind[] = {
        0, 1, 3,
        1, 2, 3
    };

    mesh = new mesh_2d(GL_STATIC_DRAW, skin, vert, sizeof(vert), ind, sizeof(ind));
}

void player::Response(void)
{
    static double prev_t = time.GetTime();

    if (lvl == NULL)
        return;
    glm::vec2 prev_pos = pos;

    is_on_ground = false;
    for (auto i = lvl->maps.begin(); i != lvl->maps.end(); i++)
        if ((*i)->IsOnMap(pos))
            if ((*i)->im.GetPixel(pos.x - (*i)->pos.x, pos.y - (*i)->pos.y).a > 0)
            {
                is_on_ground = true;
                cur_surf = *i;
            }
    if (is_on_ground)
        MoveOnGround(prev_t);
    else
        MoveInAir(prev_t);

    prev_t = time.GetTime();
}

void player::Draw(void) const
{
    glm::mat4 model = glm::rotate(glm::translate(IDENTITY_MATRIX, glm::vec3(pos.x, pos.y, 0)), atan2(g.x, -g.y), glm::vec3(0, 0, 1));

    mesh->Draw(model, IDENTITY_MATRIX, lvl->fbo_projection);
}
