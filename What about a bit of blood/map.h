#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "image.h"
#include "plane.h"

class map
{
public:
    image im;
    texture tex;

    map(string name);
    ~map();
    glm::vec2 GetCollision(const glm::vec2 &point, const glm::vec2 &dir) const;
    glm::vec2 GetFreePlace(const glm::vec2 &point, const glm::vec2 &dir) const;
    glm::vec2 GetNormal(const glm::vec2 &point) const;
    int GetWidth(void) const;
    int GetHeight(void) const;
    void Draw(void) const;
};
