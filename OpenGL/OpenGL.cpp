﻿

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

#include "chapter9/Wave/SceneWave.h"

SceneWave* scene;

void display(void)
{
    scene->render();
    glutSwapBuffers();
}

void idle(void)
{
    glutPostRedisplay();
}

void Init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    scene = new SceneWave();
    scene->initScene();
    scene->resize(320, 320);
}

void timer(int value) {
    std::cout << value << std::endl;
    scene->update(1);
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}

int main(int argc, char* argv[])
{
    // GLFW初期化
    if (glfwInit() == GL_FALSE)
    {
        return -1;
    }
    // バージョン2.1指定
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwSwapInterval(0);

    glutInitWindowPosition(320, 320);
    glutInitWindowSize(320, 320);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("頂点変異によるサーフェイスのアニメーション");

    // GLEW初期化
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutTimerFunc(100, timer, 0);

    Init();
    glutMainLoop();


    return 0;

};

