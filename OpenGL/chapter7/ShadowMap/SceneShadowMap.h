#pragma once
#ifndef SCENEShadowMap_H
#define SCENEShadowMap_H

#include "../../chapter2/glslprogram.h"
#include "../../chapter2/vbotorus.h"
#include "../../chapter3/Mesh/VBOPlane.h"
#include "../../chapter2/teaPot.h"
#include "../../chapter4/Mesh/VBOCube.h"
#include "../../chapter4/loadImage.h"
#include <stdio.h>
//#include "../../chapter4/Mesh/VBOMesh.h"
#include "../../chapter4/Mesh/SkyBox.h"
#include "../frustum.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneShadowMap
{
private:
    GLSLProgram prog, solidProg;
    GLuint shadowFBO, pass1Index, pass2Index;

    TeaPot* teapot;
    VBOPlane* plane;
    VBOTorus* torus;

    int width, height;
    int shadowMapWidth, shadowMapHeight;

    mat4 model, view, projection;
    mat4 lightPV;
    mat4 shadowBias;
    float angle;

    Frustum* lightFrustum;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void drawScene();
    void spitOutDepthBuffer();
public:
    SceneShadowMap();

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif