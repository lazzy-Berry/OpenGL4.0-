#include "SceneWave.h"
#include "../../chapter2/defines.h"
#include <freeglut.h>
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


void SceneWave::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneWave::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter9\\Wave\\Shader\\WaveShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter9\\Wave\\Shader\\WaveShader.frag", GLSLShader::FRAGMENT))
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

SceneWave::SceneWave()
{
    width = 320;
    height = 320;
    time = 0.0f;
}

void SceneWave::initScene()
{ 
    compileAndLinkShader();  
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(13.0f, 10.0f, 200, 2);
    projection = mat4(1.0f);
    
    prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
    angle = PI / 2.0f;
}

void SceneWave::render()
{
    prog.setUniform("Time", time);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(vec3(10.0f * cos(angle), 4.0f, 10.0f * sin(angle)), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(90.0f, (float)width / height, 0.3f, 100.0f);

    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Shininess", 100.0f);
    model = mat4(1.0f);
    model *= glm::rotate(-10.0f, vec3(0.0f, 0.0f, 1.0f));
    model *= glm::rotate(90.0f, vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    plane->render();
}

void SceneWave::update(float t)
{
    time += t;
    if (time >= 100)
    {
        time = 0;
    }
}

void SceneWave::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(90.0f, (float)width / height, 0.3f, 100.0f);
}
