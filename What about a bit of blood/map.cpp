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

timer WorldTime;

map::map(char * name, const std::function<glm::mat4(float)> move_f, unsigned int draw_mode): im(name)
{
	moving = move_f;
	tex.BindImg(im);
	float ratio = (float)MapHeight / MapWidth;
	float vert[] = {
		          0, GetHeight(), 0, 0, 1,
		          0,           0, 0, 0, 0,
		 GetWidth(),           0, 0, 1, 0,
  	 	 GetWidth(), GetHeight(), 0, 1, 1
	};
	GLuint ind[] = {
		0, 1, 3,
		1, 2, 3
	};

	mesh = new mesh_2d(draw_mode, tex, vert, sizeof(vert), ind, sizeof(ind));
}

map::~map()
{
    delete(&im);
    delete(&tex);
}

glm::vec2 map::GetCollision(const glm::vec2 &map_point, const glm::vec2 &dir) const
{
    glm::vec2 v(map_point.x, map_point.y), d = dir;
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
            res.x = (int)v.x, res.y = (int)v.y;
            return res;
        }
        v += d;
    }
    res.x = (int)v.x, res.y = (int)v.y;
    return res;
}

glm::vec2 map::GetFreePlace(const glm::vec2& map_point, const glm::vec2& dir) const
{
    glm::vec2 v(map_point.x, map_point.y), d = dir;
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

glm::vec2 map::GetNormal(const glm::vec2 &map_point, int D) const
{
    glm::vec2 p((int)map_point.x, (int)map_point.y);

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

void map::Draw(glm::mat4 &view) const
{
    glm::mat4 model = moving(WorldTime.GetTime());
    mesh->Draw(model, view, lvl->fbo_projection);
}

bool map::IsOnMap(glm::vec2 pos) const
{
	glm::vec2 map_pos = ToMapBasis(pos);

    if (map_pos.x >= 0 && map_pos.x < GetWidth() && map_pos.y >= 0 && map_pos.y < GetHeight())
        return true;
    return false;
}

glm::vec2 map::ToMapBasis(glm::vec2 pos, float w) const
{
	glm::vec4 res = glm::inverse(moving(WorldTime.GetTime())) * glm::vec4(pos.x, pos.y, 0, w);
	return glm::vec2(res.x, res.y);
}

glm::vec2 map::ToDefaultBasis(glm::vec2 map_pos, float w) const
{
	glm::vec4 res = moving(WorldTime.GetTime()) * glm::vec4(map_pos.x, map_pos.y, 0, w);
	return glm::vec2(res.x, res.y);
}

bool map::IsAlpha(glm::vec2 pos) const
{
	glm::vec2 map_pos = ToMapBasis(pos);
	return im.GetPixel(map_pos.x, map_pos.y).a == 0;
}

/*
	dir < 0 - LEFT
  	dir > 0 - RIGHT
*/
glm::vec2 map::Step(glm::vec2 pos_on_cur_map, float dir, float *res_len)
{
	const static glm::vec2 op[] = { glm::vec2(0, 1),
                                    glm::vec2(1, 1),
                                    glm::vec2(1, 0),
                                    glm::vec2(1, -1),
                                    glm::vec2(0, -1),
                                    glm::vec2(-1, -1),
                                    glm::vec2(-1, 0),
                                    glm::vec2(-1, 1) };
    const static int len_op = sizeof(op) / sizeof(glm::vec2);
    if (res_len != NULL)
        *res_len = 0;
	if (dir == 0)
		return pos_on_cur_map;
	float co[len_op], si[len_op];
	glm::vec2 n = GetNormal(pos_on_cur_map);
	int i_max_cos = -1;
	for (int i = 0; i < len_op; i++)
	{
		co[i] = n.x * op[i].x + n.y * op[i].y,
		si[i] = n.x * op[i].y - n.y * op[i].x;
        if (i % 2)
            co[i] /= glm::root_two<float>();
		if (si[i] * dir < 0 && (i_max_cos == -1 || co[i] > co[i_max_cos]))
			i_max_cos = i;
	}
    for (int i = len_op; i < 2 * len_op && i > 0; i += glm::sign(dir))
    {
        glm::vec2 v = op[(i_max_cos + i) % len_op] + pos_on_cur_map;
        if (im.GetPixel(v.x, v.y).a > 0)
        {
            if (res_len != NULL)
                *res_len = (i_max_cos + i) % 2 ? glm::root_two<float>() : 1.f;
            return v;
        }
    }
	return pos_on_cur_map;
}
