#pragma once
#ifndef SCENEMultiTexture_H
#define SCENEMultiTexture_H

#include "../../chapter2/glslprogram.h"
#include "../../chapter2/vbotorus.h"
#include "../../chapter3/Mesh/VBOPlane.h"
#include "../../chapter2/teaPot.h"
#include "../Mesh/VBOCube.h"
#include "../loadImage.h"
#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneMultiTexture
{
private:
    GLSLProgram prog;
    int width, height;

    VBOPlane* plane;
    VBOCube* cube;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    
    //JPEG* jpeg;
public:
    SceneMultiTexture()
    {};

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif