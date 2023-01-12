#pragma once
#ifndef SCENEDIRECTIONNALIGHT_H
#define SCENEDIRECTIONNALIGHT_H

#include "../../chapter2/glslprogram.h"
#include "../../chapter2/vbotorus.h"
#include "../Mesh/VBOPlane.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneDirectionalLight
{
private:
    GLSLProgram prog;
    int width, height;
    float angle;
    VBOTorus* torus;
    VBOPlane* plane;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();
    

public:
    SceneDirectionalLight() : angle(0.0)
    {};

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif