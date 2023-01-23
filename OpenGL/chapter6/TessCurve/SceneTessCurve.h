#pragma once
#ifndef SCENETessCurve_H
#define SCENETessCurve_H

#include "../../chapter2/glslprogram.h"
#include "../../chapter2/vbotorus.h"
#include "../../chapter3/Mesh/VBOPlane.h"
#include "../../chapter2/teaPot.h"
#include "../../chapter4/Mesh/VBOCube.h"
#include "../../chapter4/loadImage.h"
#include <stdio.h>
#include "../Mesh/vbomeshadj.h"
#include "../../chapter4/Mesh/VBOMesh.h"
#include "../../chapter4/Mesh/SkyBox.h"
#include "../Mesh/vboteapotpatch.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneTessCurve
{
private:
    GLSLProgram prog;
    GLSLProgram solidProg;

    int width, height;
    GLuint vaoHandle;

    VBOTeapotPatch* teapot;

    mat4 model;
    mat4 view,viewport;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneTessCurve()
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