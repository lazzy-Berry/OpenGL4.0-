#include "SceneSeamlessNoiseTex.h"
#include "../../chapter2/defines.h"
#include "SeamlessNoiseTex.h"
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


SceneSeamlessNoiseTex::SceneSeamlessNoiseTex()
{
    width = 800;
    height = 800;
}

void SceneSeamlessNoiseTex::initScene()
{ 
    glClearColor(0.0, 0.0, 0.0, 1.0);
    noiseTex = SeamlessNoiseTex::generate2DTex(100, 100);
}

void SceneSeamlessNoiseTex::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);

    glEnable(GL_TEXTURE_2D);//テクスチャ有効
    glBindTexture(GL_TEXTURE_2D, noiseTex);
    glEnable(GL_ALPHA_TEST);//アルファテスト開始
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex2d(10, 1000);//左下
    glTexCoord2f(0.0f, 1.0f); glVertex2d(10, 10);//左上
    glTexCoord2f(1.0f, 1.0f); glVertex2d(1000, 10);//右上
    glTexCoord2f(1.0f, 0.0f); glVertex2d(1000, 1000);//右下
    glEnd();
    glDisable(GL_ALPHA_TEST);//アルファテスト終了
    glDisable(GL_TEXTURE_2D);//テクスチャ無効

    glutSwapBuffers();
}

void SceneSeamlessNoiseTex::update(float t)
{
    //angle += 0.001f;
    //if (angle > TWOPI) angle -= TWOPI;
}

void SceneSeamlessNoiseTex::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}
