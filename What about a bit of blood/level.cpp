#include <list>
#include <windows.h>
#include <iostream>

#include <GL/glew.h> 

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>   
#else
#include <GLUT/GLUT.h>  
#endif

#include "text.h"
#include "player.h"

#include "level.h"

level *lvl;
int MapWidth, MapHeight;

level::level(string map_name): time(), fbo_texture(false), view(1.f)
{     
    fbo_texture.BindNullImg(WidthScreen, HeightScreen);

    glGenFramebuffersEXT(1, &fbo);
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fbo);
    glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
        GL_TEXTURE_2D, fbo_texture, 0);

    GLenum status = glCheckFramebufferStatusEXT(GL_DRAW_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        switch (status)
        {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            std::cout << "FBO GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT";  
                break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            std::cout << "FBO GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT";
                break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            std::cout << "FBO GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT";
                break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            std::cout << "FBO GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT";
                break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            std::cout << "FBO GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT";
                break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            std::cout << "FBO GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT";
                break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            std::cout << "FBO GL_FRAMEBUFFER_UNSUPPORTED_EXT";
                break;
        default:
            std::cout << "FBO Unknown error";
                break;
        }
        exit(0);
    }
    //glClearColor(0.1, 0.5, 0.1, 1);
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
    /*glGenRenderbuffers(1, &rboColorId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboColorId);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_RGB8, width, height);*/
    maps.push_back(new map("map1.png"));
    MapWidth = (*maps.begin())->GetWidth(), MapHeight = (*maps.begin())->GetHeight();
    const float screen_ratio = (float)WidthScreen / HeightScreen;
    p = new plane(screen_ratio);
    projection = glm::perspective(glm::radians(45.f), screen_ratio, 0.1f, 100.0f);
    const double plane_ratio = (float)MapWidth / MapHeight;
    fbo_projection = glm::ortho(0.f, (float)MapWidth, 0.f, (float)MapHeight, -1.f, 1.f);
    maps.push_back(new map("rect.png", [](float t) {return glm::translate(glm::rotate(glm::translate(IDENTITY_MATRIX,
        glm::vec3(600 + 100*(sin(t*10)-1), 400, 0)),
        t, glm::vec3(0, 0, 1)),
        glm::vec3(-100, -50, 0)); }));
}

level::~level()
{
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteFramebuffersEXT(1, &fbo);
    delete(&fbo_texture);
    delete(&p);
}

void level::DrawFBO(void) const
{
    const float screen_ratio = (float)WidthScreen / HeightScreen;
    const float plane_ratio = (float)MapWidth / MapHeight;

    static float prev_t = lvl->time.GetTime();
    static glm::vec2 down = A->g;
    glm::vec2 t = A->g - down;
    down += t * (lvl->time.GetTime() - prev_t);

    glm::mat4 view = IDENTITY_MATRIX;
    view = glm::translate(view, glm::vec3(MapWidth / 2,MapHeight / 2, 0.f));
    view = glm::scale(view, glm::vec3((float)MapWidth / WidthScreen, (float)MapHeight / HeightScreen, 1));
    view = glm::rotate(view, -atan2(down.x, -down.y), glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::translate(view, glm::vec3(-A->pos.x, -A->pos.y, 0.f));
    glUseProgram(0);
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fbo);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glViewport(0, 0, WidthScreen, HeightScreen);
    //glOrtho(-plane_ratio, plane_ratio, -1, 1, -1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    for (auto i = maps.begin(); i != maps.end(); i++)
        (*i)->Draw(view);
    A->Draw(view);

    /*glColor3d(1, 1, 0);
    glBegin(GL_TRIANGLES);
    glVertex2d(0, 1);
    glVertex2d(0, 0);
    glVertex2d(1, 0);
    glEnd();*/ 


    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glLoadIdentity();
    /*glColor3f(1, 1, 1);
    //glRasterPos2d(-1, 0.95);
    PrintText("%lf", 1 / (timerFPS.GetTime() - prev_t));
    prev_t = timerFPS.GetTime();
    glColor3f(0, 0, 0);*/
    prev_t = lvl->time.GetTime();
    glFlush();
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
}

void level::ResponseCamera(void)
{
    view = glm::translate(IDENTITY_MATRIX, glm::vec3(0.f, 0.f, -2.f));
}

void level::Draw(void)
{   
    DrawFBO();
    ResponseCamera();
    p->Draw(fbo_texture, projection, view, IDENTITY_MATRIX);
  }
