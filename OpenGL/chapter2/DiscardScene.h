#pragma once
#ifndef SCENEDISCARD_H
#define SCENEDISCARD_H

#include "glslprogram.h"
#include "teaPot.h"
#include "vbotorus.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneDiscard
{
private:
    GLSLProgram prog;
    int width, height;
    TeaPot* pot;
    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneDiscard();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif