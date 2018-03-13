#pragma once

#include <string>
#include <glm/vec2.hpp>
 
using namespace std;

class RGBA
{
public:
    unsigned char r, g, b, a;
    RGBA() : r(0), g(0), b(0), a(0) {}
    RGBA(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na) : r(nr), g(ng), b(nb), a(na) {}
    RGBA &operator=(RGBA col) { b = col.b, g = col.g, r = col.r, a = col.a; return *this; }
    bool operator==(RGBA col) const { return b == col.b && g == col.g && r == col.r && a == col.a; }
    bool operator!=(RGBA col) const { return b != col.b || g != col.g || r != col.r || a != col.a; }
    unsigned char &operator[](const int i) { return (i == 0 ? r : (i == 1 ? g : (i == 2 ? b : a))); }
};

class image
{
protected:
    int width, height;
    unsigned char *pixels;
public:

    image(string name);
    ~image();
    int GetWidth(void) const;
    int GetHeight(void) const;
    RGBA GetPixel(int x, int y) const {
        return (y >= 0 && y < height) && (x >= 0 && x < width) ?
            RGBA(pixels[(x + y * width) * 4], pixels[(x + y * width) * 4 + 1], pixels[(x + y * width) * 4 + 2],
                pixels[(x + y * width) * 4 + 3]) : RGBA();
    }

    int GetNumOfNotCromaPoints(const glm::vec2 &point) const;

    friend class plane;
    friend class texture;
};

