#pragma once

#include "image.h"

class texture
{
private:
    int width, height;
    unsigned int id;
    bool is_mipmap;
public:
    texture(const bool is_mipmap = true);
    void BindImg(const image &im);
    void BindNullImg(const int w, const int h);
    ~texture();
    int GetWidth(void) const;
    int GetHeight(void) const;
    unsigned int GetID(void) const;
    operator unsigned int() const;
};