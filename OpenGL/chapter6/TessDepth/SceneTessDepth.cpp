#include "SceneTessDepth.h"
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

void SceneTessDepth::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    //prog.setUniform("ProjectionMatrix", projection);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ViewportMatrix", viewport);
}

void SceneTessDepth::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter6\\TessDepth\\Shader\\TessDepthShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\TessDepth\\Shader\\TessDepthShader.gs", GLSLShader::GEOMETRY))
    {
        printf("Tess evaluation shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\TessDepth\\Shader\\TessDepthShader.frag", GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\TessDepth\\Shader\\TessDepthShader.tes", GLSLShader::TESS_EVALUATION))
    {
        printf("Tess evaluation shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\TessDepth\\Shader\\TessDepthShader.tcs", GLSLShader::TESS_CONTROL))
    {
        printf("Tess control shader failed to compile!\n%s",
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

void SceneTessDepth::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    teapot = new VBOTeapotPatch();
    angle = PI / 2.0;
    ///////////// Uniforms ////////////////////
    prog.setUniform("MinTessLevel", 2);
    prog.setUniform("MaxTessLevel", 15);
    prog.setUniform("MaxDepth", 20.0f);
    prog.setUniform("MinDepth", 2.0f);
    prog.setUniform("LineWidth", 0.8f);
    prog.setUniform("LineColor", vec4(0.05f, 0.0f, 0.05f, 1.0f));
    prog.setUniform("LightPosition", vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("LightIntensity", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Kd", vec3(0.9f, 0.9f, 1.0f));
    /////////////////////////////////////////////

    glPatchParameteri(GL_PATCH_VERTICES, 16);
}

void SceneTessDepth::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(6.25f * cos(angle), 6.0f, 6.25f * sin(angle));
    view = glm::lookAt(cameraPos,
        vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-2.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, -1.5f, 0.0f));
    model = glm::rotate(model, -90.0f, vec3(1.0f, 0.0f, 0.0f));
    model *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
    setMatrices();
    teapot->render();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(2.0f, 0.0f, -5.0f));
    model = glm::translate(model, vec3(0.0f, -1.5f, 0.0f));
    model = glm::rotate(model, -90.0f, vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    teapot->render();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(7.0f, 0.0f, -10.0f));
    model = glm::translate(model, vec3(0.0f, -1.5f, 0.0f));
    model = glm::rotate(model, -90.0f, vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    teapot->render();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(17.0f, 0.0f, -20.0f));
    model = glm::translate(model, vec3(0.0f, -1.5f, 0.0f));
    model = glm::rotate(model, -90.0f, vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    teapot->render();
    glFinish();
}

void SceneTessDepth::update(float t)
{
    //angle += 0.001f;
    //if (angle > TWOPI) angle -= TWOPI;
}

void SceneTessDepth::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;

    float w2 = w / 2.0f;
    float h2 = h / 2.0f;
    viewport = mat4(vec4(w2, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, h2, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));
    projection = glm::perspective(90.0f, (float)w / h, 0.3f, 30.0f);
}
