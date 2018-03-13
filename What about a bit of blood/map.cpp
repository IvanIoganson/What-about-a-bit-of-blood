#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>   
#else
#include <GLUT/GLUT.h>  
#endif

#include "map.h"

map::map(string name) : im(name), tex()
{
    tex.BindImg(im);    
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

    glm::vec2 n(0, 0);
    glm::vec2 res(0, 0);
    for (int x = point.x - D; x <= point.x + D; x++)
        for (int y = point.y - D; y <= point.y + D; y++)
            if (x >= 0 && x < im.GetWidth() && y >= 0 && y < im.GetHeight() && im.GetPixel(x, y).a == 0)
            {
                glm::vec2 t(x, y);
                n += glm::normalize(t - point);
            }
    res.x = n.x, res.y = n.y;
/*#ifdef _DEBUG
    glBegin(GL_LINES);
        glColor3d(0, 1, 0);
        glVertex2i(point.x / im.GetWidth() * 2 - 1, point.y / im.GetHeight() * 2 - 1);
        glVertex2i((point.x + n.x) / im.GetWidth() * 2 - 1, (point.y + n.y) / im.GetHeight() * 2 - 1);
    glEnd();
#endif*/
    res = glm::normalize(res);
    return res;
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
    glColor3d(1, 1, 1);

    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0); glVertex2d(-1, -1);
    glTexCoord2d(0, 1); glVertex2d(-1, 1);
    glTexCoord2d(1, 1); glVertex2d(1, 1);
    glTexCoord2d(1, 0); glVertex2d(1, -1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

}
