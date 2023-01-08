#pragma once
#ifndef SCENESUBROUTINE_H
#define SCENESUBROUTINE_H

#include "glslprogram.h"
#include "teaPot.h"
#include "vbotorus.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneSubRoutine
{
private:
    GLSLProgram prog;
    int width, height;
    TeaPot* pot;
    VBOTorus* torus;
    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneSubRoutine();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif