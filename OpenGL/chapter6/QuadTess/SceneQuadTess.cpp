#include "SceneQuadTess.h"
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

void SceneQuadTess::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    //prog.setUniform("ProjectionMatrix", projection);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ViewportMatrix", viewport);
}

void SceneQuadTess::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter6\\QuadTess\\Shader\\QuadTessShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\QuadTess\\Shader\\QuadTessShader.gs", GLSLShader::GEOMETRY))
    {
        printf("Tess evaluation shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\QuadTess\\Shader\\QuadTessShader.frag", GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\QuadTess\\Shader\\QuadTessShader.tes", GLSLShader::TESS_EVALUATION))
    {
        printf("Tess evaluation shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\QuadTess\\Shader\\QuadTessShader.tcs", GLSLShader::TESS_CONTROL))
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

void SceneQuadTess::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    float c = 3.5f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);

    angle = PI / 2.0;
    ///////////// Uniforms ////////////////////
    prog.setUniform("Inner", 4);
    prog.setUniform("Outer", 4);
    prog.setUniform("LineWidth", 1.5f);
    prog.setUniform("LineColor", vec4(0.05f, 0.0f, 0.05f, 1.0f));
    prog.setUniform("QuadColor", vec4(1.0f, 1.0f, 1.0f, 1.0f));
    /////////////////////////////////////////////

    // Set up patch VBO
    float v[] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };

    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), v, GL_STATIC_DRAW);

    // Set up patch VAO
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Set the number of vertices per patch.  IMPORTANT!!
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    GLint maxVerts;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxVerts);
    printf("Max patch vertices: %d\n", maxVerts);
}

void SceneQuadTess::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(1.5f * cos(angle), 0.0f, 1.5f * sin(angle));
    view = glm::lookAt(cameraPos,
        vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));

    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_PATCHES, 0, 4);

    glFinish();
}

void SceneQuadTess::update(float t)
{
    angle += 0.001f;
    if (angle > TWOPI) angle -= TWOPI;
}

void SceneQuadTess::resize(int w, int h)
{
}
