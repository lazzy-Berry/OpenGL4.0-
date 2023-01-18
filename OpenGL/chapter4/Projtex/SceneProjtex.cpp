#include "SceneProjtex.h"
#include "../../chapter2/defines.h"


using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#define WIDTH 1024
#define HEIGHT 1024

void SceneProjtex::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelMatrix", model);
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneProjtex::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter4\\Projtex\\Shader\\ProjtexShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter4\\Projtex\\Shader\\ProjtexShader.frag", GLSLShader::FRAGMENT))
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

void SceneProjtex::initScene()
{
    compileAndLinkShader();
    glEnable(GL_DEPTH_TEST);

    teapot = new TeaPot(14, mat4(1.0f));
    plane = new VBOPlane(100.0f, 100.0f, 1, 1);

    projection = mat4(1.0f);
    angle = (float)(TO_RADIANS(90.0));

    vec3 projPos = vec3(2.0f, 5.0f, 5.0f);
    vec3 projAt = vec3(-2.0f, -4.0f, 0.0f);
    vec3 projUp = vec3(0.0f, 1.0f, 0.0f);
    mat4 projView = glm::lookAt(projPos, projAt, projUp);
    mat4 projProj = glm::perspective(30.0f, 1.0f, 0.2f, 1000.0f);
    mat4 projScaleTrans = glm::translate(vec3(0.5f)) * glm::scale(vec3(0.5f));
    prog.setUniform("ProjectorMatrix", projScaleTrans * projProj * projView);
 
    
    //テクスチャをロード
    const char* texName = "chapter4\\Texture\\flower.png";
    Texture2D* timg = new Texture2D(texName, FIF_PNG);

    //テクスチャをOpenGLにコピー
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg->getWidth(), timg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, timg->getBits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    prog.setUniform("ProjectorTex", 0);

    prog.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
}

void SceneProjtex::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    prog.setUniform("Material.Kd", 0.5f, 0.2f, 0.1f);
    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -1.5f, 0.0f));
    model = glm::rotate(model, -90.0f, vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    teapot->render();

    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f, -0.75f, 0.0f));
    setMatrices();
    plane->render();
}

void SceneProjtex::update(float t)
{
    angle += 0.01f;
    if (angle > TWOPI) angle -= TWOPI;
}

void SceneProjtex::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(90.0f, (float)w / h, 0.3f, 100.0f);
}
