#include <windows.h>

#include <GL/glew.h>

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>   
#else
#include <GLUT/GLUT.h>  
#endif

#include <SOIL.h>

#include "image.h"

image::image(string name)
{
    pixels = SOIL_load_image(name.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

    if (pixels == NULL)
    {
        printf("Error to load image %s\n", name.c_str());
        return;
    }
}

image::~image()
{
    if (pixels != NULL)
        SOIL_free_image_data(pixels);
}

int image::GetWidth(void) const
{
    return width;
}

int image::GetHeight(void) const
{
    return height;
}

int image::GetNumOfNotCromaPoints(const glm::vec2 &point) const
{
    int res = 0;
    for (int i = point.x - 1; i <= point.x + 1; i++)
        for (int j = point.y - 1; j <= point.y + 1; j++)
            if (i >= 0 && i < width && j >= 0 && j < height && GetPixel(i, j).a > 0)
                res++;
    return res;
}
