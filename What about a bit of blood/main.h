#pragma once

//#define GLEW_STATIC
#include <GL/glew.h> 

#include <GLFW/glfw3.h>

#include "level.h"

void Init(void);
void Display(void);
void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mode);

extern GLFWwindow* Window;