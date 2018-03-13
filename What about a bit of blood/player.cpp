#include <iostream>

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>   
#else
#include <GLUT/GLUT.h>  
#endif

#include "player.h"

player *A;

void player::MoveOnGround(float prev_t)
{
    glm::vec2 n = lvl->map.GetNormal(pos);
    g = -n * MAX_PLAYER_GRAVITY;
    if (go_x != 0)
    {
        glm::vec2 d;
        float len = MAX_PLAYER_MOVE_SPEED * (time.GetTime() - prev_t);
        for (int i = 0; i < len + 1; i++)
        {
            d = glm::vec2(n.y * go_x, -n.x * go_x);
            pos += d;
            n = lvl->map.GetNormal(pos);
            g = -n * MAX_PLAYER_GRAVITY;
            if (lvl->map.im.GetPixel(pos.x, pos.y).a == 0)
            {
                pos = lvl->map.GetCollision(pos, g);
            }
            else if (lvl->map.im.GetNumOfNotCromaPoints(pos) > 8)
            {
                pos = lvl->map.GetFreePlace(pos, n);
            }
        }
        //pos += v * (time.GetTime() - prev_t);

        v *= 0;
    }
    if (jump_state == FALL)
        jump_state = ON_GROUND;
    if (jump_state == JUMP)
    {
        n = lvl->map.GetNormal(pos);
        v = n * PLAYER_JUMP;
        g = -n * MAX_PLAYER_GRAVITY;
        is_on_ground = false;
        while (lvl->map.im.GetPixel(pos.x, pos.y).a > 0)
            pos += n;
    }
}

void player::MoveInAir(float prev_t)
{
    glm::vec2 nv, npos, normg = glm::normalize(g);
    v += glm::vec2(-MAX_PLAYER_MOVE_SPEED_IN_AIR * normg.y * go_x, MAX_PLAYER_MOVE_SPEED_IN_AIR * normg.x * go_x) + g * (time.GetTime() - prev_t);
    npos = pos + v * (time.GetTime() - prev_t);

    if (jump_state == JUMP)
        jump_state = FALL;

    glm::vec2 d = glm::normalize(npos - pos);
    while (pos.y >= 0 && pos.y < lvl->map.GetHeight() && pos.x >= 0 && pos.x < lvl->map.GetWidth() && glm::dot(d, (npos - pos)) > 0)
    {
/*#ifdef _DEBUG
        glBegin(GL_POINTS);
        glColor3d(1, 0, 1);
        glVertex2i(pos.x / lvl->map.GetWidth() * 2 - 1, pos.y / lvl->map.GetHeight() * 2 - 1);
        glEnd();
#endif*/
        if (lvl->map.im.GetPixel(pos.x, pos.y).a > 0)
            break;
        pos += d;
    }
}

player::player(level *nlvl): skin(), lvl(nlvl), pos(400,300), g(0, MAX_PLAYER_GRAVITY), is_on_ground(false), go_x(0), jump_state(FALL)
{
    image im("player.png");
    skin.BindImg(im);
}

glm::vec2 player::GetTexCoord(void) const
{
    return glm::vec2(pos.x / lvl->map.GetWidth() * 2 - 1, (pos.y / lvl->map.GetHeight() * 2 - 1) / lvl->map.GetWidth() * lvl->map.GetHeight());
}

void player::Response(void)
{
    static double prev_t = time.GetTime();

    if (lvl == NULL)
        return;

    is_on_ground = (lvl->map.im.GetPixel(pos.x, pos.y).a > 0);
#ifdef _DEBUG
    /*printf("%d %d (%lf %lf): %d %d %d %d\n", (int)pos.x, (int)pos.y, 
        GetTexCoord().x, GetTexCoord().y,
        lvl->map.im.GetPixel(pos.x, pos.y).r,
        lvl->map.im.GetPixel(pos.x, pos.y).g, lvl->map.im.GetPixel(pos.x, pos.y).b,
        lvl->map.im.GetPixel(pos.x, pos.y).a);*/
#endif
    if (is_on_ground)
        MoveOnGround(prev_t);
    else
        MoveInAir(prev_t);

    prev_t = time.GetTime();
}

void player::Draw(void) const
{
    glPushMatrix();

    glColor3d(1, 1, 1);
    glTranslated(GetTexCoord().x, GetTexCoord().y, 0);
    glRotated(atan2(g.x, -g.y) * 180 / glm::pi<double>(), 0, 0, 1);
    
    glBindTexture(GL_TEXTURE_2D, skin);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2d(0 - (float)skin.GetWidth() / lvl->map.GetWidth() / 2, 0);
        glTexCoord2d(0, 0); glVertex2d(0 - (float)skin.GetWidth() / lvl->map.GetWidth() / 2, 0 + (float)skin.GetHeight() / lvl->map.GetHeight());
        glTexCoord2d(1, 0); glVertex2d(0 + (float)skin.GetWidth() / lvl->map.GetWidth() / 2, 0 + (float)skin.GetHeight() / lvl->map.GetHeight());
        glTexCoord2d(1, 1); glVertex2d(0 + (float)skin.GetWidth() / lvl->map.GetWidth() / 2, 0);
    glEnd();

    glPopMatrix();
}
