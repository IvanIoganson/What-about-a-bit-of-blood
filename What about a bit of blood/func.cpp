#include <iostream>
#include <ctime>

#include "main.h"
#include "image.h"
#include "player.h"
#include "plane.h"
#include "text.h"
#include "mesh_2d.h"

int WidthScreen = 1920, HeightScreen = 1080;
double ratio;
GLFWwindow* Window;

void Init(void)
{
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#else
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, false);
#endif 
    //glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const  GLFWvidmode * mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    WidthScreen = mode->width;
    HeightScreen = mode->height;
    Window = glfwCreateWindow(WidthScreen, HeightScreen, "Window", monitor, nullptr);

    if (Window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(0);
    }
    glfwGetFramebufferSize(Window, &WidthScreen, &HeightScreen);
    glfwMakeContextCurrent(Window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(0);
    } 
    ratio = WidthScreen / (double)HeightScreen;
    glViewport(0, 0, WidthScreen, HeightScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, WidthScreen, HeightScreen); 

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_MULTISAMPLE); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glfwSwapInterval(1);

    mesh_2d::DefaultShader = new shader("Mesh_Default/vertex_shader.glsl", "Mesh_Default/fragment_shader.glsl");
    lvl = new level("map1.png");
    A = new player(lvl);
}

void SynchronizeTimers(void)
{
    clock_t ttt = clock();
    lvl->time.SynchronizeTimer(ttt);
    A->time.SynchronizeTimer(ttt);
    WorldTime.SynchronizeTimer(ttt);
    lvl->p->time.SynchronizeTimer(ttt);
}

void Display(void)
{
    //system("cls");
    glClearColor(0.05, 0.05, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);

    static timer timerFPS;
    static double prev_t = timerFPS.GetTime();
    timerFPS.SynchronizeTimer(clock());
    SynchronizeTimers();
    glLoadIdentity();

    A->Response();
    lvl->Draw();

    glUseProgram(0);

    cout << 1 / (timerFPS.GetTime() - prev_t) << endl;
    prev_t = timerFPS.GetTime();
    //Sleep(16);
    /*glLoadIdentity();
    glColor3f(1, 1, 1);
    glRasterPos2d(-1, 0.95);
    PrintText("%lf", 1 / (timerFPS.GetTime() - prev_t));
    prev_t = timerFPS.GetTime();
    glColor3f(0, 0, 0);*/ 
}

void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_A:
            A->moving = player::LEFT;
            break;
        case GLFW_KEY_D:
            A->moving = player::RIGHT;
            break;
        case GLFW_KEY_W:
            if(A->jump_state == player::ON_GROUND)
                A->jump_state = player::JUMP;
            break;
        default:
            break;
        }

    else if (action == GLFW_RELEASE)
        switch (key)
        {
        case GLFW_KEY_A:
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
                A->moving = player::STOP;
            else
                A->moving = player::RIGHT;
            break;
        case GLFW_KEY_D:
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
                A->moving = player::STOP;
            else
                A->moving = player::LEFT;
            break;
        case GLFW_KEY_W:
            if (A->jump_state == player::JUMP || A->jump_state == player::MOVE_UP)
                A->jump_state = player::FALL;
            break;
        default:
            break;
        }
}