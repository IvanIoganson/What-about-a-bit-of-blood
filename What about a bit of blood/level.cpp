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

level::level(string map_name): map(map_name), time(), fbo_texture(false), camera(1.f), p((float)map.GetWidth() / map.GetHeight())
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
}

level::~level()
{
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteFramebuffersEXT(1, &fbo);
    delete(&fbo_texture);
    delete(&p);
}

void level::DrawFBO(void)
{
    static timer timerFPS;
    static double prev_t = timerFPS.GetTime();

    glUseProgram(0);
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fbo);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    //glViewport(0, 0, WidthScreen, HeightScreen);
    /*float ratio = (float)WidthScreen / HeightScreen;
    if (ratio <= 1)
        glOrtho(-1.0, 1.0, -1.0 / ratio, 1.0 / ratio, -1.0, 1.0);
    else
        glOrtho(-1.0*ratio, 1.0*ratio, -1.0, 1.0, -1.0, 1.0);*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    map.Draw();

    A->Draw(); 

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glLoadIdentity();
    /*glColor3f(1, 1, 1);
    //glRasterPos2d(-1, 0.95);
    PrintText("%lf", 1 / (timerFPS.GetTime() - prev_t));
    prev_t = timerFPS.GetTime();
    glColor3f(0, 0, 0);
    /*glColor4d(1, 0, 0, 1);
    glBegin(GL_POINTS);
        glVertex2d(A->GetTexCoord().x, A->GetTexCoord().y);
    glEnd();*/

    /*glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
        GL_TEXTURE_2D, texture, 0);*/
    glFlush();
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
}

void level::ResponseCamera(void)
{
    static const float screen_ratio = (float)WidthScreen / HeightScreen, plane_ratio = (float)map.GetWidth() / map.GetHeight();
    static glm::vec2 down = A->g;
    glm::vec2 t = A->g - down;
    down += t * 0.01f;

    //camera = glm::ortho(-screen_ratio, screen_ratio, -1.f, 1.f, -1.f, 1.f);
    camera = glm::perspective(glm::radians(45.f), screen_ratio, 0.1f, 100.0f);
    
    camera = glm::rotate(camera, -atan2(down.x, -down.y), glm::vec3(0.0f, 0.0f, 1.0f));
    camera = glm::translate(camera, glm::vec3(-A->GetTexCoord().x * plane_ratio, -A->GetTexCoord().y, -3.f));
}

void level::Draw(void)
{   
    DrawFBO();
    ResponseCamera();
    p.Draw(fbo_texture, camera);
}
