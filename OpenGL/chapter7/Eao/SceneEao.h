#pragma once
#ifndef SCENEEao_H
#define SCENEEao_H

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

class SceneEao
{
private:
    GLSLProgram prog;
    GLuint shadowFBO, pass1Index, pass2Index;

    TeaPot* teapot;
    VBOPlane* plane;
    VBOTorus* torus;
    VBOMesh7* orga;

    int width, height;

    mat4 model, view, projection;
    vec3 lightPos;
    float angle;

   
    void setMatrices();
    void compileAndLinkShader();
    void drawScene();

public:
    SceneEao();

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif