#include "MultiTexture.h"
#include "../../chapter2/defines.h"


using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#define WIDTH 1024
#define HEIGHT 1024

void SceneMultiTexture::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneMultiTexture::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter4\\MultiTexture\\Shader\\MultiTextureShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter4\\MultiTexture\\Shader\\MultiTextureShader.frag", GLSLShader::FRAGMENT))
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

void SceneMultiTexture::initScene()
{
    compileAndLinkShader();
    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(50.0f, 50.0f, 1, 1);
    cube = new VBOCube();

    view = glm::lookAt(vec3(1.0f, 1.25f, 1.25f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    angle = 0.0;
    prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
    
    GLuint textIDs[2];
    glGenTextures(2, textIDs);

    //�����K�e�N�X�`�������[�h
    const char* texName = "chapter4\\Texture\\brick1.jpg";
    Texture2D* jpeg = new Texture2D(texName, FIF_JPEG);

    //�����K�e�N�X�`����OpenGL�ɃR�s�[
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textIDs[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, jpeg->getWidth(), jpeg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, jpeg->getBits());

    prog.setUniform("BrickTex", 0);

    //�R�P�e�N�X�`���t�@�C�������[�h
    const char* mossName = "chapter4\\Texture\\moss.png";
    Texture2D* mossImg = new Texture2D(mossName, FIF_PNG);

    //�ۃe�N�X�`����OpenGL�ɃR�s�[
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textIDs[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mossImg->getWidth(), mossImg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, mossImg->getBits());

    prog.setUniform("MossTex", 1);
}

void SceneMultiTexture::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    setMatrices();
    cube->render();
}

void SceneMultiTexture::update(float t)
{
    angle += 0.01f;
    if (angle > TWOPI) angle -= TWOPI;
}

void SceneMultiTexture::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(90.0f, (float)w / h, 0.3f, 100.0f);
}
