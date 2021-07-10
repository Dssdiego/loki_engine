//
// Created by Diego Santos Seabra on 27/06/21.
//

#pragma once

#include <iostream>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#include <fstream>
#include <vector>

class Shader
{
public:
    Shader();

    Shader(std::string vertexShaderName, std::string fragmentShaderName);

    void use();
    void cleanUp();
private:
    GLuint mProgram;
    GLuint mVertexShader;
    GLuint mFragmentShader;
};

