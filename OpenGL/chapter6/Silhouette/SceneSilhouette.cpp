#include "SceneSilhouette.h"
#include "../../chapter2/defines.h"

using glm::vec3;

using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>

void SceneSilhouette::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ViewportMatrix", viewport);
}

void SceneSilhouette::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter6\\Silhouette\\Shader\\SilhouetteShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\Silhouette\\Shader\\SilhouetteShader.gs", GLSLShader::GEOMETRY))
    {
        printf("Geometry shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\Silhouette\\Shader\\SilhouetteShader.frag", GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.link())
    {
        printf("Shader program failed to link!\n%s",
            prog.log().c_str());
        exit(1);
    }

    if (!prog.validate())
    {
        printf("Program failed to validate!\n%s",
            prog.log().c_str());
        exit(1);
    }

    prog.use();
}

void SceneSilhouette::setupFBO()
{
}

void SceneSilhouette::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    ogre = new VBOMeshAdj("chapter4\\Mesh\\bs_ears.obj");

    angle = PI / 2.0;
   
    prog.setUniform("EdgeWidth", 0.015f);
    prog.setUniform("PctExtend", 0.25f);
    prog.setUniform("LineColor", vec4(0.05f, 0.0f, 0.05f, 1.0f));
    prog.setUniform("Material.Kd", 0.7f, 0.5f, 0.2f);
    prog.setUniform("Light.Position", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Light.Intensity", 1.0f, 1.0f, 1.0f);
}

void SceneSilhouette::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(3.0f * cos(angle), 0.0f, 3.0f * sin(angle));
    view = glm::lookAt(cameraPos,
        vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));

    model = mat4(1.0f);
    setMatrices();
    ogre->render();

    glFinish();
}

void SceneSilhouette::update(float t)
{
    angle += 0.01f;
    if (angle > TWOPI) angle -= TWOPI;
}

void SceneSilhouette::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    
    projection = glm::perspective(90.0f, (float)w / h, 0.3f, 100.0f);
   /* float w2 = width / 2.0f;
    float h2 = height / 2.0f;
    viewport = mat4(vec4(w2, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, h2, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));*/
}
