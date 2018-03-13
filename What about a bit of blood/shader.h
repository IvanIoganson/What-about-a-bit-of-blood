#pragma once

class shader
{
public:
    unsigned int program;
    shader(const char *VertexShaderPath, const char *FragmentShaderPath);
    void Use(void) const;
    ~shader();
};