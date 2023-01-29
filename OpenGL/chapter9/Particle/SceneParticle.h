#pragma once
#ifndef SCENEParticle_H
#define SCENEParticle_H

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

class SceneParticle
{
private:
    GLSLProgram prog;

    int width, height;
    GLuint initVel, startTime, particles;
    GLuint nParticles;
    VBOPlane* plane;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;
    float time;

    void setMatrices();
    void compileAndLinkShader();
   void initBuffers();
    float randFloat();
public:
    SceneParticle();

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif