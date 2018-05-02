#include <GL/glew.h> 

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>   
#else
#include <GLUT/GLUT.h>  
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "level.h"
#include "map.h"

map::map(string name) : im(name), tex(), pos(0, 0)
{
    tex.BindImg(im);  
    float vert[] = {
        0         , GetHeight(), 0, 0, 1,
        0         ,           0, 0, 0, 0,
        GetWidth(),           0, 0, 1, 0,
        GetWidth(), GetHeight(), 0, 1, 1
    };
    GLuint ind[] = {
        0, 1, 3,
        1, 2, 3
    };

    mesh = new mesh_2d(GL_STATIC_DRAW, tex, vert, sizeof(vert), ind, sizeof(ind));
}

map::map(string name, glm::vec2 &init_pos) : im(name), pos(init_pos)
{
    tex.BindImg(im);
    float ratio = (float)MapHeight / MapWidth;
    float vert[] = {
        0         , GetHeight(), 0, 0, 1,
        0         ,           0, 0, 0, 0,
        GetWidth(),           0, 0, 1, 0,
        GetWidth(), GetHeight(), 0, 1, 1
    };
    GLuint ind[] = {
        0, 1, 3,
        1, 2, 3
    };

    mesh = new mesh_2d(GL_STATIC_DRAW, tex, vert, sizeof(vert), ind, sizeof(ind));
}


map::~map()
{
    delete(&im);
    delete(&tex);
}

glm::vec2 map::GetCollision(const glm::vec2 &point, const glm::vec2 &dir) const
{
    glm::vec2 v(point.x - pos.x, point.y - pos.y), d = dir;
    glm::vec2 res(0, 0);
    d = glm::normalize(d);
    while (v.y >= 0 && v.y < im.GetHeight() && v.x >= 0 && v.x < im.GetWidth())
    {
        res.x = (int)v.x, res.y = (int)v.y;
/*#ifdef _DEBUG
        glBegin(GL_POINTS);
            glColor3d(1, 0, 0);
            glVertex2i(res.x / GetWidth() * 2 - 1, res.y / GetHeight() * 2 - 1);
        glEnd();
#endif*/
        if (im.GetPixel(res.x, res.y).a > 0)
        {
            res.x = (int)(v.x + pos.x), res.y = (int)(v.y + pos.y);
            return res;
        }
        v += d;
    }
    res.x = (int)(v.x + pos.x), res.y = (int)(v.y + pos.y);
    return res;
}

glm::vec2 map::GetFreePlace(const glm::vec2& point, const glm::vec2& dir) const
{
    glm::vec2 v(point.x - pos.x, point.y - pos.y), d = dir;
    glm::vec2 res;
    d = glm::normalize(d);
    while (v.y >= 0 && v.y < im.GetHeight() && v.x >= 0 && v.x < im.GetWidth())
    {
        res.x = (int)v.x, res.y = (int)v.y;
/*#ifdef _DEBUG
        glBegin(GL_POINTS);
        glColor3d(0, 0, 1);
        glVertex2i(res.x, res.y);
        glEnd();
#endif*/
        if (im.GetPixel(res.x, res.y).a == 0)
        {
            v -= d;
            res.x = (int)(v.x + pos.x), res.y = (int)(v.y + pos.y);
            return res;
        }
        v += d;
    }
    res.x = (int)(v.x + pos.x), res.y = (int)(v.y + pos.y);
    return res;
}

glm::vec2 map::GetNormal(const glm::vec2 &point) const
{
    const int D = 5;
    glm::vec2 p((int)(point.x - pos.x), (int)(point.y - pos.y));

    glm::vec2 n(0, 0);
    for (int x = p.x - D; x <= p.x + D; x++)
        for (int y = p.y - D; y <= p.y + D; y++)
            if (x >= 0 && x < im.GetWidth() && y >= 0 && y < im.GetHeight() && im.GetPixel(x, y).a == 0)
            {
                glm::vec2 t(x, y);
                    if (t != p)
                n += glm::normalize(t - p);
            }
    if (n == glm::vec2(0, 0))
        return n;
    return glm::normalize(n);
}

int map::GetWidth(void) const
{
    return im.GetWidth();
}

int map::GetHeight(void) const
{
    return im.GetHeight();
}

void map::Draw(void) const
{
    glm::mat4 model = glm::translate(IDENTITY_MATRIX, glm::vec3(pos.x, pos.y, 0.f));
    mesh->Draw(model, IDENTITY_MATRIX, lvl->fbo_projection);
}

bool map::IsOnMap(glm::vec2 pos) const
{
    if (pos.x >= this->pos.x && pos.x <= this->pos.x + GetWidth() && pos.y >= this->pos.y && pos.y <= this->pos.y + GetHeight())
        return true;
    return false;
}
