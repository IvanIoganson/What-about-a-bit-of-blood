#pragma once

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>   
#else
#include <GLUT/GLUT.h>  
#endif

#include "shader.h"

class mesh_2d
{
private:
    GLfloat *vertices;
    GLuint *indices;
    size_t size_of_vertices, size_of_indices;
    unsigned int VAO, VBO, EBO, tex, draw_mode, shaderProgram;
    GLint projectionLoc, viewLoc, modelLoc;
public:
    static shader *DefaultShader;
    mesh_2d(unsigned int draw_mode, unsigned int textur = 0,  
        GLfloat *vertices = NULL, size_t size_of_vertices = 0, GLuint *indices = NULL, size_t size_of_indices = 0, unsigned int shad = 0);
    ~mesh_2d();
    void Draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const;
    void mesh_2d::Response(GLfloat *vertices = NULL, size_t size_of_vertices = 0, GLuint *indices = NULL, size_t size_of_indices = 0);
    void BindTexture(unsigned int textur);
};

#define IDENTITY_MATRIX glm::mat4(1., 0., 0., 0., \
                                  0., 1., 0., 0., \
                                  0., 0., 1., 0., \
                                  0., 0., 0., 1.)