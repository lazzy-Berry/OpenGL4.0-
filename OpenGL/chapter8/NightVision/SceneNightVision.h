#pragma once
#ifndef SCENENightVision_H
#define SCENENightVision_H

#include "../../chapter2/glslprogram.h"
#include "../../chapter2/vbotorus.h"
#include "../../chapter3/Mesh/VBOPlane.h"
#include "../../chapter2/teaPot.h"
#include "../../chapter4/Mesh/VBOCube.h"
#include "../../chapter4/loadImage.h"
#include <stdio.h>
#include "../../chapter7/VBOMesh7.h"
#include "../../chapter4/Mesh/SkyBox.h"
#include "../../chapter7/frustum.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneNightVision
{
private:
    GLSLProgram prog;

    int width, height;
    GLuint fsQuad, pass1Index, pass2Index;
    GLuint renderFBO;
    GLuint renderTex;
    GLuint noiseTex;

    TeaPot* teapot;
    VBOPlane* plane;
    VBOTorus* torus;
    VBOCube* cube;

    mat4 model, view, projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void pass1();
    void pass2();

public:
    SceneNightVision();

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif