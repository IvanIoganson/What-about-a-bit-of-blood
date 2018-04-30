#include <iostream>

#include <GL/glew.h> 

#include <glm/matrix.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include "plane.h"

plane::plane(float ratio): shaderProgram("Plane/vertex_shader.glsl", "Plane/fragment_shader.glsl"), time()
{
    this->ratio = ratio;

    vertexTimeLocation = glGetUniformLocation(shaderProgram.program, "time");
    projectionLoc = glGetUniformLocation(shaderProgram.program, "projection");
    viewLoc = glGetUniformLocation(shaderProgram.program, "view");
    modelLoc = glGetUniformLocation(shaderProgram.program, "model");

    GLfloat vertices[] = {
        1.f * ratio,  1.f, 0.0f, 1.0f, 1.0f,
        1.f * ratio, -1.f, 0.0f, 1.0f, 0.0f,
        -1.f * ratio, -1.f, 0.0f, 0.0f, 0.0f,
        -1.f * ratio,  1.f, 0.0f, 0.0f, 1.0f
    };
    GLuint indices[] = {  
        0, 1, 3,   
        1, 2, 3    
    };
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); */
    glBindVertexArray(0);
}

void plane::Draw(const texture &tex, const glm::mat4 &projection, const glm::mat4 &view, const glm::mat4 &model) const
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glColor3d(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    shaderProgram.Use();
    glUniform1f(vertexTimeLocation, time.GetTime());
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    /*glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2d(-1, -1);
        glTexCoord2d(0, 0); glVertex2d(-1, 1);
        glTexCoord2d(1, 0); glVertex2d(1, 1);
        glTexCoord2d(1, 1); glVertex2d(1, -1);
    glEnd();*/

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

plane::~plane(void)
{
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
