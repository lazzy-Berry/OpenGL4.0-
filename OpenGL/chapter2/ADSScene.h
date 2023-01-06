#pragma once
#pragma once
#ifndef ADSSCENE_H
#define ADSSCENE_H

#include "glslprogram.h"
#include "vbotorus.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
using glm::mat4;

class ADSScene
{
private:
    GLSLProgram prog;
    int width, height;
    VBOTorus* torus;
    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    ADSScene();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEADS_H