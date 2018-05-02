#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "image.h"
#include "plane.h"
#include "mesh_2d.h"

class map
{
public:
    image im;
    texture tex;
    mesh_2d *mesh;
    glm::vec2 pos;

    map(string name);
    map(string name, glm::vec2 &init_pos);
    ~map();
    glm::vec2 GetCollision(const glm::vec2 &point, const glm::vec2 &dir) const;
    glm::vec2 GetFreePlace(const glm::vec2 &point, const glm::vec2 &dir) const;
    glm::vec2 GetNormal(const glm::vec2 &point) const;
    int GetWidth(void) const;
    int GetHeight(void) const;
    virtual void Draw(void) const;
    bool IsOnMap(glm::vec2 pos) const;
};
