#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>

#include "image.h"
#include "plane.h"
#include "mesh_2d.h"

class map
{
public:
    image im;
    texture tex;
    mesh_2d *mesh;
	std::function<glm::mat4(float)> moving;

	map::map(char *name,
		const std::function<glm::mat4(float)> move_f = [](float t) {return IDENTITY_MATRIX; },
		unsigned int draw_mode = GL_STATIC_DRAW);
    ~map();
    glm::vec2 GetCollision(const glm::vec2 &map_point, const glm::vec2 &dir) const;
    glm::vec2 GetFreePlace(const glm::vec2 &map_point, const glm::vec2 &dir) const;
    glm::vec2 GetNormal(const glm::vec2 &map_point, int D = 1) const;
    int GetWidth(void) const;
    int GetHeight(void) const;
    virtual void Draw(glm::mat4 &view) const;
	bool IsOnMap(glm::vec2 pos) const;
	glm::vec2 ToMapBasis(glm::vec2 pos, float w = 1.f) const;
	glm::vec2 ToDefaultBasis(glm::vec2 map_pos, float w = 1.f) const;
	bool IsAlpha(glm::vec2 pos) const;
	glm::vec2 Step(glm::vec2 pos_on_cur_map, float dir, float *res_len = NULL);
};

extern timer WorldTime;
