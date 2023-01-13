#pragma once
#ifndef SCENEFog_H
#define SCENEFog_H

#include "../../chapter2/glslprogram.h"
#include "../../chapter2/vbotorus.h"
#include "../Mesh/VBOPlane.h"
#include "../../chapter2/teaPot.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneFog
{
private:
    GLSLProgram prog;
    int width, height;

    VBOTorus* torus;
    VBOPlane* plane;
    TeaPot* teapot;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    

public:
    SceneFog()
    {};

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif