#include "scene.h"

#include <cstdio>
#include <cstdlib>

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


void Scene::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void Scene::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter2\\Shader\\DiffuseShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter2\\Shader\\DiffuseShader.frag", GLSLShader::FRAGMENT))
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

Scene::Scene()
{
}

void Scene::initScene()
{
    compileAndLinkShader();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    torus = new VBOTorus(0.7f, 0.3f, 50, 50);

    model = mat4(1.0f);
    model *= glm::rotate(-35.0f, vec3(1.0f, 0.0f, 0.0f));
    model *= glm::rotate(35.0f, vec3(0.0f, 1.0f, 0.0f));
    view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("LightPosition", view * vec4(5.0f, 5.0f, 2.0f, 1.0f));
}

void Scene::update(float t)
{
}

void Scene::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setMatrices();
    torus->render();
}

void Scene::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(70.0f, (float)w / h, 0.3f, 100.0f);
}