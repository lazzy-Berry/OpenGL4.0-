#include "DiscardScene.h"

#include <cstdio>
#include <cstdlib>

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


void SceneDiscard::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneDiscard::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter2\\Shader\\DiscardShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter2\\Shader\\DiscardShader.frag", GLSLShader::FRAGMENT))
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

SceneDiscard::SceneDiscard() : angle(0.0f)
{
}

void SceneDiscard::initScene()
{
    compileAndLinkShader();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    pot = new TeaPot(13, mat4(1.0f));

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

void SceneDiscard::update(float t)
{
}

void SceneDiscard::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec4 lightPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    prog.setUniform("Light.Position", lightPos);

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0, -1.5, 0.0));
    model *= glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.5f));
    //model *= glm::rotate(140.0f, vec3(0.0f,1.0f,0.0f));
    model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    setMatrices();
    pot->render();
}

void SceneDiscard::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(20.0f, (float)w / h, 0.3f, 100.0f);
}
