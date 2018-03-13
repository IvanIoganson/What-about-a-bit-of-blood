#include <iostream>
#include "main.h"
#include "timer.h"
#include "text.h"
#include "image.h"

using namespace std;

int main(int argc, char * argv[])
{
    Init();
    //BuildFont();
    glfwSetKeyCallback(Window, Keyboard);

    while (!glfwWindowShouldClose(Window))
    {
        glfwPollEvents();
        Display();
        glfwSwapBuffers(Window);
    }

    //KillFont();

    glfwDestroyWindow(Window);
    glfwTerminate();
    return 0;
    
    /*glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    
    glutInitWindowSize(1920, 1280);
    glutCreateWindow("Window");
    glutFullScreen();

    BuildFont();

    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialFunc(SpecialKeyboard);

    glutMainLoop();

    KillFont();*/
    return 0;
}
