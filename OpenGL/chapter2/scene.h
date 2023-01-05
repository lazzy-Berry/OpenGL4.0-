#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "glslprogram.h"
#include "vbotorus.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
using glm::mat4;

class Scene
{
private:
    GLSLProgram prog;
    int width, height;
    VBOTorus* torus;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    Scene();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEADS_H