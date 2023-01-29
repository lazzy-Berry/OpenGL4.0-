#include "SceneNoiseTex.h"
#include "../../chapter2/defines.h"
#include "NoiseTex.h"
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


SceneNoiseTex::SceneNoiseTex()
{
    width = 800;
    height = 800;
}

void SceneNoiseTex::initScene()
{ 
    glClearColor(0.0, 0.0, 0.0, 1.0);
    noiseTex = NoiseTex::generate2DTex(100, 100);
}

void SceneNoiseTex::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);

    glEnable(GL_TEXTURE_2D);//�e�N�X�`���L��
    glBindTexture(GL_TEXTURE_2D, noiseTex);
    glEnable(GL_ALPHA_TEST);//�A���t�@�e�X�g�J�n
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex2d(10, 1000);//����
    glTexCoord2f(0.0f, 1.0f); glVertex2d(10, 10);//����
    glTexCoord2f(1.0f, 1.0f); glVertex2d(1000, 10);//�E��
    glTexCoord2f(1.0f, 0.0f); glVertex2d(1000, 1000);//�E��
    glEnd();
    glDisable(GL_ALPHA_TEST);//�A���t�@�e�X�g�I��
    glDisable(GL_TEXTURE_2D);//�e�N�X�`������

    glutSwapBuffers();
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
