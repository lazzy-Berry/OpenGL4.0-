#pragma once
#ifndef SCENENormalMap_H
#define SCENENormalMap_H

#include "../../chapter2/glslprogram.h"
#include "../../chapter2/vbotorus.h"
#include "../../chapter3/Mesh/VBOPlane.h"
#include "../../chapter2/teaPot.h"
#include "../Mesh/VBOCube.h"
#include "../loadImage.h"
#include <stdio.h>
#include "../Mesh/VBOMesh.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneNormalMap
{
private:
    GLSLProgram prog;
    int width, height;

    VBOPlane* plane;
    VBOCube* cube;
    VBOMesh* ogre;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    
    //JPEG* jpeg;
public:
    SceneNormalMap()
    {};

    void initScene();
    void render();
    void update(float t);
    void resize(int w, int h);
};

#endif