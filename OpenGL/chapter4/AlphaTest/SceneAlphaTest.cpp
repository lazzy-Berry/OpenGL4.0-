#include "SceneAlphaTest.h"
#include "../../chapter2/defines.h"


using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#define WIDTH 1024
#define HEIGHT 1024

void SceneAlphaTest::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneAlphaTest::compileAndLinkShader()
{
    if (!prog.compileShaderFromFile("chapter4\\AlphaTest\\Shader\\AlphaTestShader.vert", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
            prog.log().c_str());
        exit(1);
    }
    if (!prog.compileShaderFromFile("chapter4\\AlphaTest\\Shader\\AlphaTestShader.frag", GLSLShader::FRAGMENT))
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

void SceneAlphaTest::initScene()
{
    compileAndLinkShader();
    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(50.0f, 50.0f, 1, 1);
    teapot = new TeaPot(14, mat4(1.0f));

    projection = mat4(1.0f);
    angle = 0.0;
    prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
    
    GLuint textIDs[2];
    glGenTextures(2, textIDs);

    //ブロックテクスチャをロード
    const char* texName = "chapter4\\Texture\\cement.jpg";
    Texture2D* jpeg = new Texture2D(texName, FIF_JPEG);

    //ブロックテクスチャをOpenGLにコピー
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textIDs[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, jpeg->getWidth(), jpeg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, jpeg->getBits());

    prog.setUniform("BaseTex", 0);

    //コケテクスチャファイルをロード
    const char* mossName = "chapter4\\Texture\\moss.png";
    Texture2D* mossImg = new Texture2D(mossName, FIF_PNG);

    //苔テクスチャをOpenGLにコピー
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textIDs[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mossImg->getWidth(), mossImg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, mossImg->getBits());

    prog.setUniform("AlphaTex", 1);
}

void SceneAlphaTest::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3(6.0f * cos(angle), 0.25f, 6.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    prog.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -1.5f, 0.0f));
    model = glm::rotate(model, -90.0f, vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    teapot->render();
}

void SceneAlphaTest::update(float t)
{
    angle += 0.01f;
    if (angle > TWOPI) angle -= TWOPI;
}

void SceneAlphaTest::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(90.0f, (float)w / h, 0.3f, 100.0f);
}
