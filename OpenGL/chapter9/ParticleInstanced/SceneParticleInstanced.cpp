#include "SceneParticleInstanced.h"
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


void SceneParticleInstanced::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("ProjectionMatrix", projection);
}

void SceneParticleInstanced::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter9\\ParticleInstanced\\Shader\\ParticleInstancedShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter9\\ParticleInstanced\\Shader\\ParticleInstancedShader.frag", GLSLShader::FRAGMENT))
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

void SceneParticleInstanced::initBuffers()
{
    nParticles = 500;

    glGenBuffers(1, &initVel);
    glGenBuffers(1, &startTime);

    int size = nParticles * 3 * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

    vec3 v(0.0f);
    float velocity, theta, phi;
    GLfloat* data = new GLfloat[nParticles * 3];
    for (int i = 0; i < nParticles; i++) {

        theta = glm::mix(0.0f, (float)PI / 6.0f, randFloat());
        phi = glm::mix(0.0f, (float)TWOPI, randFloat());

        v.x = sinf(theta) * cosf(phi);
        v.y = cosf(theta);
        v.z = sinf(theta) * sinf(phi);

        velocity = glm::mix(1.25f, 1.5f, randFloat());
        v = glm::normalize(v) * velocity;

        data[3 * i] = v.x;
        data[3 * i + 1] = v.y;
        data[3 * i + 2] = v.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

    // Fill the first start time buffer
    delete[] data;
    data = new GLfloat[nParticles];
    float time = 0.0f;
    float rate = 0.01f;
    for (int i = 0; i < nParticles; i++) {
        data[i] = time;
        time += rate;
    }
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete[] data;

    // Create vertex arrays for each set of buffers
    glBindVertexArray(torus->getVertexArrayHandle());
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(4);

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);
}

float SceneParticleInstanced::randFloat()
{
    return ((float)rand() / RAND_MAX);
}

SceneParticleInstanced::SceneParticleInstanced()
{
    width = 320;
    height = 320;
    time = 0.0f;
    deltaT = 0.0f;
}

void SceneParticleInstanced::initScene()
{ 
    compileAndLinkShader();  

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(13.0f, 10.0f, 200, 2);
    float c = 0.15;
    torus = new VBOTorus(0.7f * c, 0.3f * c, 20, 20);
    projection = mat4(1.0f);

    angle = PI / 2.0f;
    model = mat4(1.0f);

    initBuffers();

    prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("ParticleLifetime", 3.5f);
    prog.setUniform("Gravity", vec3(0.0f, -0.2f, 0.0f));
}

void SceneParticleInstanced::render()
{
    prog.setUniform("Time", time);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(vec3(3.0f * cos(angle), 1.5f, 3.0f * sin(angle)), vec3(0.0f, 1.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    prog.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.2f);
    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);
    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(torus->getVertexArrayHandle());
    glDrawElementsInstanced(GL_TRIANGLES, 6 * 20 * 20, GL_UNSIGNED_INT, 0, nParticles);
}

void SceneParticleInstanced::update(float t)
{
    deltaT = t - time;
    time += t;
}

void SceneParticleInstanced::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(90.0f, (float)width / height, 0.3f, 100.0f);
}
