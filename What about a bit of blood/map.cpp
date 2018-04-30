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

map::map(string name) : im(name), tex()
{
    tex.BindImg(im);  
    float ratio = (float)GetHeight() / GetWidth();
    float vert[] = {
        -1, ratio , 0, 0, 1,
        -1, -ratio, 0, 0, 0,
        1 , -ratio, 0, 1, 0,
        1 , ratio , 0, 1, 1
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
    glm::vec2 v(point.x, point.y), d = dir;
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
            return res;
        v += d;
    }
    res.x = (int)v.x, res.y = (int)v.y;
    return res;
}

glm::vec2 map::GetFreePlace(const glm::vec2& point, const glm::vec2& dir) const
{
    glm::vec2 v(point.x, point.y), d = dir;
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
            res.x = (int)v.x, res.y = (int)v.y;
            return res;
        }
        v += d;
    }
    res.x = (int)v.x, res.y = (int)v.y;
    return res;
}

glm::vec2 map::GetNormal(const glm::vec2 &point) const
{
    const int D = 5;
    glm::vec2 p((int)point.x, (int)point.y);

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
    mesh->Draw(IDENTITY_MATRIX, IDENTITY_MATRIX, lvl->fbo_projection);
}
