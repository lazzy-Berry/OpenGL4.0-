#pragma once
#ifndef SCENEJitter_H
#define SCENEJitter_H

#include "../../chapter2/glslprogram.h"
#include "../../chapter2/vbotorus.h"
#include "../../chapter3/Mesh/VBOPlane.h"
#include "../../chapter2/teaPot.h"
#include "../../chapter4/Mesh/VBOCube.h"
#include "../../chapter4/loadImage.h"
#include <stdio.h>
#include "../VBOMesh7.h"
#include "../../chapter4/Mesh/SkyBox.h"
#include "../frustum.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneJitter
{
private:
    GLSLProgram prog;
    GLuint shadowFBO, pass1Index, pass2Index;

    TeaPot* teapot;
    VBOPlane* plane;
    VBOTorus* torus;
    VBOMesh7* mesh;

    int width, height;
    int samplesU, samplesV;
    int jitterMapSize;
    float radius;
    int shadowMapWidth, shadowMapHeight;

    mat4 model, view, projection;
    mat4 lightView,lightProjection,lightPV;
    mat4 shadowScale;
    vec3 lightPos;
    float angle;

    Frustum* lightFrustum;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void drawScene();
    void drawBuildingScene();
    float jitter();
    void buildJitterTex();

public:
    SceneJitter();

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif