#include "SceneRefractCube.h"
#include "../../chapter2/defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <iostream>
#include <cstdio>
#include <cstdlib>

void SceneRefractCube::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneRefractCube::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter4\\RefractCube\\Shader\\RefractCubeShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter4\\RefractCube\\Shader\\RefractCubeShader.frag", GLSLShader::FRAGMENT))
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

void SceneRefractCube::loadCubeMap(const char* baseFileName)
{
   // glEnable(GL_TEXTURE);
    glActiveTexture(GL_TEXTURE0);
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    const char* suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
    GLuint targets[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    for (int i = 0; i < 6; i++) {
        string texName = string(baseFileName) + "_" + suffixes[i] + ".png";
        std::cout << "Loading: " << texName << std::endl;
        Texture2D* img = new Texture2D(texName.c_str(), FIF_PNG);
        glTexImage2D(targets[i], 0, GL_RGBA, img->getWidth(), img->getHeight(),
            0, GL_RGBA, GL_UNSIGNED_BYTE, img->getBits());
    }
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    prog.setUniform("CubeMapTex", NULL);
}

void SceneRefractCube::initScene()
{
    compileAndLinkShader();
    glEnable(GL_DEPTH_TEST);

    teapot = new TeaPot(14, mat4(1.0f));
    sky = new SkyBox();
    plane = new VBOPlane(1.0f, 1.0f, 1, 1);

    projection = mat4(1.0f);
    angle = (float)(TO_RADIANS(90.0));

    loadCubeMap("chapter4\\Texture\\cubemap_night\\night");
}

void SceneRefractCube::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    prog.setUniform("WorldCameraPosition", cameraPos);

    prog.setUniform("DrawSkyBox", true);
    model = mat4(1.0f);
    setMatrices();
    sky->render();
    prog.setUniform("DrawSkyBox", false);

    prog.setUniform("MaterialColor", vec4(0.5f, 0.5f, 0.5f, 1.0f));
    prog.setUniform("ReflectFactor", 0.85f);

    model = mat4(1.0f);
    //model *= glm::scale(vec3(0.5f, 0.5f, 0.5f));
    model *= glm::translate(vec3(0.0f, -1.0f, 0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    teapot->render();
}

void SceneRefractCube::update(float t)
{
    angle += 0.01f;
    if (angle > TWOPI) angle -= TWOPI;
}

void SceneRefractCube::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(90.0f, (float)w / h, 0.3f, 100.0f);
}
