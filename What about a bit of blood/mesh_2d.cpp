#include <GL/glew.h> 

#include <glm/matrix.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include "mesh_2d.h"

shader *mesh_2d::DefaultShader;

mesh_2d::mesh_2d(unsigned int draw_mode, unsigned int textur, 
    GLfloat *vertices, size_t size_of_vertices, GLuint *indices, size_t size_of_indices, unsigned int shad):
    vertices(vertices), size_of_vertices(size_of_vertices), indices(indices), size_of_indices(size_of_indices),
    tex(textur), draw_mode(draw_mode), shaderProgram(shad)
{
    if (shaderProgram == 0) 
        shaderProgram = mesh_2d::DefaultShader->program;

    projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    modelLoc = glGetUniformLocation(shaderProgram, "model");

    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_of_indices, indices, draw_mode);
    glBufferData(GL_ARRAY_BUFFER, size_of_vertices, vertices, draw_mode);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}

mesh_2d::~mesh_2d()
{
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void mesh_2d::Draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const
{      
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glColor3d(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void mesh_2d::Response(GLfloat *vertices, size_t size_of_vertices, GLuint *indices, size_t size_of_indices)
{
    glBindVertexArray(VAO);

    if (vertices != NULL && size_of_vertices != 0)
    {
        this->vertices = vertices;
        this->size_of_vertices = size_of_vertices;
        glBufferSubData(GL_ARRAY_BUFFER, 0, size_of_vertices, vertices);
    }
    if (indices != NULL && size_of_indices != 0)
    {
        this->indices = indices;
        this->size_of_indices = size_of_indices;
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size_of_indices, indices);
    }
    glBindVertexArray(0);
}

void mesh_2d::BindTexture(unsigned int textur)
{
    tex = textur;
}
