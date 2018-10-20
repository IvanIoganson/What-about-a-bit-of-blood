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

void player::MoveOnGround(float prev_t, float cur_t)
{
	static glm::vec2 last_pos;
	static float len = 0;
    if (jump_state == FALL || jump_state == MOVE_UP)
    {
        jump_state = ON_GROUND;
        if (cur_surf->im.GetPixel(pos_on_cur_map.x, pos_on_cur_map.y).a > 0)
        {
            glm::vec2 n;
            int j = 1;
            do {
                n = cur_surf->GetNormal(pos_on_cur_map, j++);
            } while (n.x * n.x + n.y * n.y < 0.1);
            pos_on_cur_map = cur_surf->GetFreePlace(pos_on_cur_map, n);
        }
        else
        {
            pos_on_cur_map = cur_surf->GetCollision(pos_on_cur_map, v);
        }
        pos = cur_surf->ToDefaultBasis(pos_on_cur_map);
    }
    last_pos = pos;
	glm::vec2 start = pos_on_cur_map;
    glm::vec2 n;

    if (moving == LEFT)
        go_x -= (cur_t - prev_t) * PLAYER_NORMAL_ACCELERATION_X_ON_GROUND,
        go_x = go_x < -1 ? -1 : go_x;
    else if (moving == RIGHT)
        go_x += (cur_t - prev_t) * PLAYER_NORMAL_ACCELERATION_X_ON_GROUND,
        go_x = go_x > 1 ? 1 : go_x;
    else
        go_x = 0;

    if (moving != STOP)
    {
        int x = pos_on_cur_map.x, y = pos_on_cur_map.y;
		glm::vec2 d;
		len += MAX_PLAYER_MOVE_SPEED_ON_GROUND * (cur_t - prev_t) * go_x;
        while (int(len) != 0)
        {
            float len_step;
			pos_on_cur_map = cur_surf->Step(pos_on_cur_map, go_x, &len_step);
            if (len_step == 0)
                break;
			len -= glm::sign(go_x) * len_step;

            map *m = cur_surf;
            glm::vec2 p = cur_surf->ToDefaultBasis(pos_on_cur_map);
            for (auto i = lvl->maps.begin(); i != lvl->maps.end(); i++)
                if (m != *i && (*i)->IsOnMap(p))
                    if (!(*i)->IsAlpha(p))
                    {
                        cur_surf = *i;
                        pos_on_cur_map = cur_surf->ToMapBasis(p);
                        start = pos_on_cur_map;
                        n = cur_surf->GetNormal(pos_on_cur_map);
                        int j = 1;
                        do {
                            n = cur_surf->GetNormal(pos_on_cur_map, j++);
                        } while (n.x * n.x + n.y * n.y < 0.1);
                        pos_on_cur_map = cur_surf->GetFreePlace(pos_on_cur_map, n);
                        break;
                    }
        }
    }
	pos = cur_surf->ToDefaultBasis(pos_on_cur_map);
    map *m = cur_surf;
    for (auto i = lvl->maps.begin(); i != lvl->maps.end(); i++)
        if (m != *i && (*i)->IsOnMap(pos))
            if (!(*i)->IsAlpha(pos))
            {
                cur_surf = *i;
                pos_on_cur_map = cur_surf->ToMapBasis(pos);
                n = cur_surf->GetNormal(pos_on_cur_map);
                int j = 1;
                do {
                    n = cur_surf->GetNormal(pos_on_cur_map, j++);
                } while (n.x * n.x + n.y * n.y < 0.1);
                pos_on_cur_map = cur_surf->GetFreePlace(pos_on_cur_map, n);
                pos = cur_surf->ToDefaultBasis(pos_on_cur_map);
                if (!m->IsAlpha(pos))
                {
                    cur_surf = m;
                    pos_on_cur_map = cur_surf->ToMapBasis(pos);
                    n = cur_surf->GetNormal(pos_on_cur_map);
                    int j = 1;
                    do {
                        n = cur_surf->GetNormal(pos_on_cur_map, j++);
                    } while (n.x * n.x + n.y * n.y < 0.1);
                    pos_on_cur_map = cur_surf->GetFreePlace(pos_on_cur_map, n);
                    pos = cur_surf->ToDefaultBasis(pos_on_cur_map);
                }
                start = pos_on_cur_map;
                break;
            }
    v = (pos - last_pos) / (cur_t - prev_t);
    n = cur_surf->GetNormal(pos_on_cur_map, 5);
    g = cur_surf->ToDefaultBasis(-n, 0);
    if (jump_state == JUMP)
    {
		v = v - g * PLAYER_JUMP_SPEED - (pos_on_cur_map - start) / (cur_t - prev_t),
        is_on_ground = false;
        while (!cur_surf->IsAlpha(pos))
			pos -= g;
        if (glm::dot(g, v) < 0)
            MoveInAir(prev_t, cur_t);
    }
}

void player::MoveInAir(float prev_t, float cur_t)
{
    static glm::vec2 v_start(0,0);
    glm::vec2 nv, npos;
    static float jump_start_time;

    if (moving == LEFT)
        go_x -= (cur_t - prev_t) * PLAYER_NORMAL_ACCELERATION_X_IN_AIR,
        go_x = go_x < -1 ? -1 : go_x;
    else if (moving == RIGHT)
        go_x += (cur_t - prev_t) * PLAYER_NORMAL_ACCELERATION_X_IN_AIR,
        go_x = go_x > 1 ? 1 : go_x;

    if (jump_state == JUMP)		
        v_start = v,
        jump_start_time = cur_t,
        jump_state = MOVE_UP;
    if (jump_state == MOVE_UP)
        if (jump_start_time + MAX_MOVE_UP_TIME < cur_t)
            jump_state = FALL;
    if (jump_state == FALL)
        v_start += g * (cur_t - prev_t) * MAX_PLAYER_GRAVITY;

    v = glm::vec2(-MAX_PLAYER_MOVE_SPEED_IN_AIR * g.y * go_x, MAX_PLAYER_MOVE_SPEED_IN_AIR * g.x * go_x) + v_start;
    npos = pos + v * (cur_t - prev_t);

    glm::vec2 d = glm::normalize(npos - pos);
    while (glm::dot(d, (npos - pos)) > 0)
    {
		pos += d;
        for (auto i = lvl->maps.begin(); i != lvl->maps.end(); i++)
            if ((*i)->IsOnMap(pos))
                if (!(*i)->IsAlpha(pos))
                {
                    cur_surf = *i;
                    is_on_ground = true;
                    pos_on_cur_map = cur_surf->ToMapBasis(pos);
                    glm::vec2 n = cur_surf->GetNormal(pos_on_cur_map);
                    int j = 1;
                    do {
                        n = cur_surf->GetNormal(pos_on_cur_map, j++);
                    } while (n.x * n.x + n.y * n.y < 0.1);
                    pos_on_cur_map = cur_surf->GetFreePlace(pos_on_cur_map, n);
                    pos = cur_surf->ToDefaultBasis(pos_on_cur_map);
                    v_start = glm::vec2(0, 0);
                    break;
                }
        if (is_on_ground)
            break;
    }
}

player::player(level *nlvl): skin(), lvl(nlvl), pos(4000,3000), g(0, 1), is_on_ground(false),
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
    pos_on_cur_map = cur_surf->ToMapBasis(pos);
}

void player::Response(void)
{
    static double prev_t = time.GetTime();

    if (lvl == NULL)
        return;

	double cur_t = time.GetTime();
    if (is_on_ground)
        MoveOnGround(prev_t, cur_t);
    else
        MoveInAir(prev_t, cur_t);

    prev_t = cur_t;
}

void player::Draw(glm::mat4 &view) const
{
    glm::mat4 model = glm::rotate(glm::translate(IDENTITY_MATRIX, glm::vec3(pos.x, pos.y, 0)), atan2(g.x, -g.y), glm::vec3(0, 0, 1));

    mesh->Draw(model, view, lvl->fbo_projection);
}
