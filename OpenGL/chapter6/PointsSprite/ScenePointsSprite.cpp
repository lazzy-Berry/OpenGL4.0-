#include "ScenePointsSprite.h"
#include "../../chapter2/defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>

void ScenePointsSprite::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("ProjectionMatrix", projection);
}

void ScenePointsSprite::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter6\\PointsSprite\\Shader\\PointsSpriteShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\PointsSprite\\Shader\\PointsSpriteShader.gs", GLSLShader::GEOMETRY))
    {
        printf("Geometry shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter6\\PointsSprite\\Shader\\PointsSpriteShader.frag", GLSLShader::FRAGMENT))
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

void ScenePointsSprite::initScene()
{
    compileAndLinkShader();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    float c = 2.5f;

    angle = PI / 2.0;
    numSprites = 50;
    locations = new float[numSprites * 3];

    srand(time(0));
    for (int i = 0; i < numSprites; i++)
    {
        vec3 p(((float)rand() / RAND_MAX * 2.0f) - 1.0f,
            ((float)rand() / RAND_MAX * 2.0f) - 1.0f,
            ((float)rand() / RAND_MAX * 2.0f) - 1.0f);
        locations[i * 3] = p.x;
        locations[i * 3 + 1] = p.y;
        locations[i * 3 + 2] = p.z;
    }

    GLuint handle;
    glGenBuffers(1, &handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, numSprites * 3 * sizeof(float), locations, GL_STATIC_DRAW);

    delete locations;

    glGenVertexArrays(1, &sprites);
    glBindVertexArray(sprites);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    const char* texName = "chapter4\\Texture\\flower.png";
    Texture2D* spriteImg = new Texture2D(texName, FIF_PNG);

    GLuint texID;
    glGenTextures(1, &texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spriteImg->getWidth(), spriteImg->getHeight(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, spriteImg->getBits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    prog.setUniform("SpriteTex", 0);
    prog.setUniform("Size2", 0.15f);
}

void ScenePointsSprite::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(3.0f * cos(angle), 0.0f, 3.0f * sin(angle));
    view = glm::lookAt(cameraPos,
        vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));

    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(sprites);
    glDrawArrays(GL_POINTS, 0, numSprites);

    glFinish();
}

void ScenePointsSprite::update(float t)
{
    angle += 0.01f;
    if (angle > TWOPI) angle -= TWOPI;
}

void ScenePointsSprite::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(90.0f, (float)w / h, 0.3f, 100.0f);
}
