﻿
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

int main()
{
    // GLFW初期化
    if (glfwInit() == GL_FALSE)
    {
        return -1;
    }
    // ウィンドウ生成
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Test", NULL, NULL);
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

    //Shader読み込み
    GLuint program = LoadShader("chapter1\\basic.vert", "chapter1\\basic.frag");
    //プログラムをOpenGLパイプラインにインストール
    glUseProgram(program);

    GLuint vaoHandle;
    GLuint vboHandle[2];

    //ユニフォーム用のShaderをBind
    BindUniformShaders(&program, &vaoHandle, vboHandle);
    //ユニフォームブロックとユニフォームバッファオブジェクトを使う
    UseUniformBlockAndBuffer(&program);

    // フレームループ
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        // バッファのクリア
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderUseUniformBlockAndBuffer(&vaoHandle);

        // ダブルバッファのスワップ
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);

    // GLFWの終了処理
    glfwTerminate();

    return 0;
};

