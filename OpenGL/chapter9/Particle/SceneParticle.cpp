#include "SceneParticle.h"
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


void SceneParticle::setMatrices()
{
    mat4 mv = view * model;
    //prog.setUniform("ModelViewMatrix", mv);
    //prog.setUniform("NormalMatrix",
    //    mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneParticle::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter9\\Particle\\Shader\\ParticleShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter9\\Particle\\Shader\\ParticleShader.frag", GLSLShader::FRAGMENT))
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

void SceneParticle::initBuffers()
{
    nParticles = 8000;

    glGenBuffers(1, &initVel);   // Initial velocity buffer
    glGenBuffers(1, &startTime); // Start time buffer

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

    delete[] data;
    data = new GLfloat[nParticles];
    float time = 0.0f;
    float rate = 0.00075f;
    for (int i = 0; i < nParticles; i++) {
        data[i] = time;
        time += rate;
    }
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete[] data;

    glGenVertexArrays(1, &particles);
    glBindVertexArray(particles);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
   
    glBindVertexArray(0);
}

float SceneParticle::randFloat()
{
    return ((float)rand() / RAND_MAX);
}

SceneParticle::SceneParticle()
{
    width = 320;
    height = 320;
    time = 0.0f;
}

void SceneParticle::initScene()
{ 
    compileAndLinkShader();  
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    //glEnable(GL_DEPTH_TEST);
    //prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(10.0f);

    plane = new VBOPlane(13.0f, 10.0f, 200, 2);
    projection = mat4(1.0f);
    
    angle = PI / 2.0f;

    const char* texName = "chapter9\\Texture\\bluewater.png";
    Texture2D* img = new Texture2D(texName, FIF_PNG);

    initBuffers();

    GLuint texID;
    glGenTextures(1, &texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->getWidth(), img->getHeight(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, img->getBits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    prog.setUniform("ParticleTex", 0);

    prog.setUniform("ParticleLifetime", 3.5f);
    prog.setUniform("Gravity", vec3(0.0f, -0.2f, 0.0f));

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void SceneParticle::render()
{
    prog.setUniform("Time", time);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    view = glm::lookAt(vec3(3.0f * cos(angle), 1.5f, 3.0f * sin(angle)), vec3(0.0f, 1.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(particles);
    glDrawArrays(GL_POINTS, 0, nParticles);
    glDisable(GL_TEXTURE_2D);
}

void SceneParticle::update(float t)
{
    time += t;
    std::cout << time << std::endl;
    if (time >= 100)
    {
        time = 0;
    }
}

void SceneParticle::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(60.0f, (float)width / height, 0.3f, 100.0f);
}
