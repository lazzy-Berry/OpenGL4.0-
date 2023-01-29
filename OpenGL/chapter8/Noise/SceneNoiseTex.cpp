#include "SceneNoiseTex.h"
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


SceneNoiseTex::SceneNoiseTex()
{
    width = 800;
    height = 600;
}

void SceneNoiseTex::initScene()
{ 
    glEnable(GL_TEXTURE_2D);

    // 描画範囲の指定
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    
    //テクスチャのロード
    const char* texName = "chapter8\\Texture\\brick1.jpg";
    Texture2D texImg(texName, FIF_JPEG);
    glGenTextures(1, &noiseTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noiseTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImg.getWidth(), texImg.getHeight(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, texImg.getBits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

void SceneNoiseTex::render()
{

    glBindTexture(GL_TEXTURE_2D, noiseTex);

    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0); glVertex2f(-0.9, -0.9);
    glTexCoord2f(0, 1); glVertex2f(-0.9, 0.9);
    glTexCoord2f(1, 1); glVertex2f(0.9, 0.9);
    glTexCoord2f(1, 0); glVertex2f(0.9, -0.9);
    glEnd();

    
}

void SceneNoiseTex::update(float t)
{
    //angle += 0.001f;
    //if (angle > TWOPI) angle -= TWOPI;
}

void SceneNoiseTex::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}
