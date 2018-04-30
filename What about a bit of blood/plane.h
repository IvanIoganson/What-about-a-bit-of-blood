#pragma once

#include "shader.h"
#include "timer.h"
#include "image.h"
#include "texture.h"

class plane {
private:
    timer time;
    unsigned int VAO, VBO, EBO;
    shader shaderProgram;
    GLint vertexTimeLocation, viewLoc, projectionLoc, modelLoc;
    float ratio;
public:
    plane(float ratio);
    void plane::Draw(const texture &tex, const glm::mat4 &projection, const glm::mat4 &view, const glm::mat4 &model) const;
    unsigned int GetShaderID(void) const { return shaderProgram.program; }
    ~plane();
};
