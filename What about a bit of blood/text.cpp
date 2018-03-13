#include <windows.h>  
#include <stdio.h>    
#include <math.h>     
#include <stdarg.h>   
#include <gl\gl.h>    
#include <gl\glu.h>   

#include "text.h"

HDC        hDC = NULL;
HGLRC      hRC = NULL;
HWND       hWnd = NULL;
HINSTANCE  hInstance; 

GLuint   base;      

GLvoid BuildFont(GLvoid)  
{
    HFONT font;          

    base = glGenLists(96);

    font = CreateFont(-24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Courier New");
    if (hWnd == NULL)
        hWnd = GetDesktopWindow();
    if (hDC == NULL)
        hDC = GetDC(hWnd);
    SelectObject(hDC, font);       

    wglUseFontBitmaps(hDC, 32, 96, base); 
}

void PrintText(const char *fmt, ...)
{

    float length = 0;
    char text[256];
    va_list ap;

    if (fmt == NULL)
        return;

    va_start(ap, fmt);
        vsprintf(text, fmt, ap);
    va_end(ap);
    //printf("%s\n", text);
    glPushAttrib(GL_LIST_BIT);      
    glListBase(base - 32);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib(); 
}

GLvoid KillFont(GLvoid)           
{
    glDeleteLists(base, 96);     
}
