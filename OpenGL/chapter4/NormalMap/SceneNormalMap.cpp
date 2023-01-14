#include "SceneNormalMap.h"
#include "../../chapter2/defines.h"


using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#define WIDTH 1024
#define HEIGHT 1024

void SceneNormalMap::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneNormalMap::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter4\\NormalMap\\Shader\\NormalMapShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter4\\NormalMap\\Shader\\NormalMapShader.frag", GLSLShader::FRAGMENT))
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

void SceneNormalMap::initScene()
{
    compileAndLinkShader();
    glEnable(GL_DEPTH_TEST);

    ogre = new VBOMesh("chapter4\\Mesh\\bs_ears.obj");

    view = glm::lookAt(vec3(-1.0f, 0.25f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);
    angle = (float)(TO_RADIANS(100.0));

    prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
    
    GLuint textIDs[2];
    glGenTextures(2, textIDs);

    //ディフューズテクスチャをロード
    const char* texName = "chapter4\\Texture\\ogre_diffuse.png";
    Texture2D* jpeg = new Texture2D(texName, FIF_PNG);

    //ディフューズテクスチャをOpenGLにコピー
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textIDs[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, jpeg->getWidth(), jpeg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, jpeg->getBits());

    prog.setUniform("ColorTex", 0);

    //ノーマルMapテクスチャファイルをロード
    const char* mossName = "chapter4\\Texture\\ogre_normalmap.png";
    Texture2D* mossImg = new Texture2D(mossName, FIF_PNG);

    //ノーマルMapテクスチャをOpenGLにコピー
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textIDs[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mossImg->getWidth(), mossImg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, mossImg->getBits());

    prog.setUniform("NormalMapTex", 1);
}

void SceneNormalMap::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.setUniform("Light.Position", view * vec4(10.0f * cos(angle), 1.0f, 10.0f * sin(angle), 1.0f));
    prog.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);
    model = glm::scale(model, vec3(0.4f, 0.4f, 0.4f));
    //model = glm::translate(model, vec3(0.0f, -0.0f, 0.0f));
    model = glm::rotate(model, 90.0f, vec3(0.0f, 0.0f, 1.4f));
    
    setMatrices();
    ogre->render();
}

void SceneNormalMap::update(float t)
{
    angle += 0.01f;
    if (angle > TWOPI) angle -= TWOPI;
}

void SceneNormalMap::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(50.0f, (float)w / h, 0.3f, 100.0f);
}
