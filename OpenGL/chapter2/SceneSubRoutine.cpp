#include "SceneSubRoutine.h"

#include <cstdio>
#include <cstdlib>

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


void SceneSubRoutine::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneSubRoutine::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter2\\Shader\\SubRoutineShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter2\\Shader\\SubRoutineShader.frag", GLSLShader::FRAGMENT))
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

SceneSubRoutine::SceneSubRoutine() : angle(0.0f)
{
}

void SceneSubRoutine::initScene()
{
    compileAndLinkShader();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    mat4 transform = glm::translate(mat4(1.0f), vec3(1.0f, 0.1f, 0.25f));
    pot = new TeaPot(13, transform);
    torus = new VBOTorus(0.7f, 0.3f, 30, 30);

    view = glm::lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Light.La", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Light.Ls", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);
}

void SceneSubRoutine::update(float t)
{
}

void SceneSubRoutine::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec4 lightPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    prog.setUniform("Light.Position", lightPos);

    GLuint programHandle = prog.getHandle();
    GLuint adsIndex = glGetSubroutineIndex(programHandle,
        GL_VERTEX_SHADER,
        "phongModel");
    GLuint diffuseIndex = glGetSubroutineIndex(programHandle,
        GL_VERTEX_SHADER,
        "diffuseOnly");

    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &adsIndex);
    model = mat4(1.0f);
    model *= glm::translate(vec3(-2.0, -1.5, 0.0));
    model *= glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.5f));
    //model *= glm::rotate(140.0f, vec3(0.0f,1.0f,0.0f));
    model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    setMatrices();
    pot->render();

    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &diffuseIndex);
    model = mat4(1.0f);
    model *= glm::translate(vec3(2.0, -1.5, 0.0));
    model *= glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.5f));
    model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    setMatrices();
    pot->render();
}

void SceneSubRoutine::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(20.0f, (float)w / h, 0.3f, 100.0f);
}
