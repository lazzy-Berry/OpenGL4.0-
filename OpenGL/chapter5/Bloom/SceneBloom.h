#pragma once
#ifndef SCENEBloom_H
#define SCENEBloom_H

#include "../../chapter2/glslprogram.h"
#include "../../chapter2/vbotorus.h"
#include "../../chapter3/Mesh/VBOPlane.h"
#include "../../chapter2/teaPot.h"
#include "../../chapter4/Mesh/VBOCube.h"
#include "../../chapter4/loadImage.h"
#include <stdio.h>
#include "../../chapter4/Mesh/VBOMesh.h"
#include "../../chapter4/Mesh/SkyBox.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneBloom
{
private:
    GLSLProgram prog;

    int width, height;
    GLuint fsQuad, pass1Index, pass2Index, pass3Index, pass4Index;
    GLuint renderFBO, fbo1, fbo2;
    GLuint renderTex, tex1, tex2;

    VBOPlane* plane;
    VBOCube* cube;
    SkyBox* sky;
    TeaPot* teapot;
    VBOTorus* torus;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void pass1();
    void pass2();
    void pass3();
    void pass4();
    float gauss(float, float);

public:
    SceneBloom()
    {
        width = 800;
        height = 600;
    };

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif