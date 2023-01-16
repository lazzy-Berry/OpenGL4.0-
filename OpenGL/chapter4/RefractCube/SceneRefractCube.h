#pragma once
#ifndef SCENERefractCube_H
#define SCENERefractCube_H

#include "../../chapter2/glslprogram.h"
#include "../../chapter2/vbotorus.h"
#include "../../chapter3/Mesh/VBOPlane.h"
#include "../../chapter2/teaPot.h"
#include "../Mesh/VBOCube.h"
#include "../loadImage.h"
#include <stdio.h>
#include "../Mesh/VBOMesh.h"
#include "../Mesh/SkyBox.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneRefractCube
{
private:
    GLSLProgram prog;
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
    void loadCubeMap(const char* baseFileName);

    //JPEG* jpeg;
public:
    SceneRefractCube()
    {};

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif