#include "SceneEao.h"
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

void SceneEao::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    //prog.setUniform("ProjectionMatrix", projection);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneEao::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter7\\Eao\\Shader\\EaoShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter7\\Eao\\Shader\\EaoShader.frag", GLSLShader::FRAGMENT))
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

void SceneEao::drawScene()
{
    model = mat4(1.0f);
    setMatrices();
    orga->render();

    glFinish();
}

SceneEao::SceneEao()
{
    width = 800;
    height = 600;
}

void SceneEao::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    
    angle = PI/2.0f;
    
    orga = new VBOMesh7("chapter7\\Mesh\\bs_ears.obj", false, true);

    lightPos = vec3(0.0f, 0.0f, 0.0f);

    prog.setUniform("Light.Position", lightPos);
    prog.setUniform("Light.Intensity", vec3(1.0f));

    float c = 2.25f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);

    const char* texName = "chapter7\\Mesh\\ao_ears.png";
    Texture2D texImg(texName, FIF_PNG);

    GLuint texID;
    glGenTextures(1, &texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImg.getWidth(), texImg.getHeight(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, texImg.getBits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    prog.setUniform("AOTex", 0);

    const char* difftexName = "chapter7\\Mesh\\diffuse.png";
    Texture2D diffTexImg(difftexName, FIF_PNG);

    glGenTextures(1, &texID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, diffTexImg.getWidth(), diffTexImg.getHeight(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, diffTexImg.getBits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    prog.setUniform("DiffTex", 1);
}

void SceneEao::render()
{
    view = glm::lookAt(vec3(3.0f * cos(angle), 0.0f, 3.0f * sin(angle)), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glFinish();
}

void SceneEao::update(float t)
{
    angle += 0.001f;
    if (angle > TWOPI) angle -= TWOPI;
}

void SceneEao::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}
