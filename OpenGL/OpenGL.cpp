

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <freeglut.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "chapter8/SeamlessNoise/SceneSeamlessNoiseTex.h"

SceneSeamlessNoiseTex* scene;

void display(void)
{
    scene->render();
}

void idle(void)
{
    glutPostRedisplay();
}

void Init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    scene = new SceneSeamlessNoiseTex();
    scene->initScene();
}

int main(int argc, char* argv[])
{
    glutInitWindowPosition(320, 320);
    glutInitWindowSize(320, 320);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("シームレスなノイズテクスチャの作成");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    Init();
    glutMainLoop();
    return 0;

};

