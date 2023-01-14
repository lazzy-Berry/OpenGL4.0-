
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "chapter1/GLShader.hpp"

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "chapter2/scene.h"

#include "chapter2/ADSScene.h"
#include "chapter2/SceneTwoSide.h"
#include "chapter2/FlatScene.h"
#include "chapter2/SceneSubRoutine.h"
#include "chapter2/DiscardScene.h"
#include "chapter3/multiLight/SceneMultiLight.h"
#include "chapter3/directionalLight/SceneDirectionalLight.h"
#include "chapter3/PerFragment/ScenePerFragment.h"
#include "chapter3/SpotLight/SceneSpotLight.h"
#include "chapter3/Toon/SceneToon.h"
#include "chapter3/Fog/SceneFog.h"
#include "chapter4/UseTexture/SceneTexture.h"
#include "chapter4/MultiTexture/MultiTexture.h"
#include "chapter4/AlphaTest/SceneAlphaTest.h"
#include "chapter4/NormalMap/SceneNormalMap.h"

int main()
{
    // GLFW初期化
    if (glfwInit() == GL_FALSE)
    {
        return -1;
    }
    // ウィンドウ生成
    GLFWwindow* window = glfwCreateWindow(1280, 640, "OpenGL Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    // バージョン2.1指定
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);


    // GLEW初期化
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    //timer用
    double  prev = glfwGetTime();
    const  double  TIME = 0.1;

    //法線マップを使う
    SceneNormalMap* scene = new SceneNormalMap();

    scene->initScene();
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    scene->resize(1280, 640);

    // フレームループ
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        // バッファのクリア
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene->render();

        //Timerイベント
        double  now = glfwGetTime();
        if (now - prev > TIME)
        {
            prev = now;
            scene->update(TIME);
        }

        // ダブルバッファのスワップ
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete scene;
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);

    // GLFWの終了処理
    glfwTerminate();

    return 0;
};

