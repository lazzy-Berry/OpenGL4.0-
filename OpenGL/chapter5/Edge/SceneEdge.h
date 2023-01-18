#pragma once
#ifndef SCENEEdge_H
#define SCENEEdge_H

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

class SceneEdge
{
private:
    GLSLProgram prog;

    GLuint fsQuad, pass1Index, pass2Index, fboHandle;
    int width, height;

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

public:
    SceneEdge()
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