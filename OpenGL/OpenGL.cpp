
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

//Shaderファイルの読み込み
int readShaderSource(GLuint shaderObj, std::string fileName)
{
    //ファイルの読み込み
    std::ifstream ifs(fileName);
    if (!ifs)
    {
        std::cout << "error" << std::endl;
        return -1;
    }

    std::string source;
    std::string line;
    while (getline(ifs, line))
    {
        source += line + "\n";
    }

    // シェーダのソースプログラムをシェーダオブジェクトへ読み込む
    const GLchar* sourcePtr = (const GLchar*)source.c_str();
    GLint length = source.length();
    glShaderSource(shaderObj, 1, &sourcePtr, &length);

    return 0;
}

//Shaderオブジェクト生成
GLint makeShader(std::string vertexFileName, std::string fragmentFileName)
{
    // シェーダーオブジェクト作成
    GLuint vertShaderObj = glCreateShader(GL_VERTEX_SHADER);
    //GLuint fragShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint shader;

    // シェーダーコンパイルとリンクの結果用変数
    GLint compiled, linked;

    /* シェーダーのソースプログラムの読み込み */
    if (readShaderSource(vertShaderObj, vertexFileName)) return -1;
    //if (readShaderSource(fragShaderObj, fragmentFileName)) return -1;

    /* バーテックスシェーダーのソースプログラムのコンパイル */
    glCompileShader(vertShaderObj);
    glGetShaderiv(vertShaderObj, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        fprintf(stderr, "Compile error in vertex shader.\n");
        return -1;
    }

    /* フラグメントシェーダーのソースプログラムのコンパイル */
    //glCompileShader(fragShaderObj);
    //glGetShaderiv(fragShaderObj, GL_COMPILE_STATUS, &compiled);
    //if (compiled == GL_FALSE)
    //{
    //    fprintf(stderr, "Compile error in fragment shader.\n");
    //    return -1;
    //}

    /* プログラムオブジェクトの作成 */
    shader = glCreateProgram();

    /* シェーダーオブジェクトのシェーダープログラムへの登録 */
    glAttachShader(shader, vertShaderObj);
    //glAttachShader(shader, fragShaderObj);

    /* シェーダーオブジェクトの削除 */
    glDeleteShader(vertShaderObj);
    //glDeleteShader(fragShaderObj);

    /* シェーダープログラムのリンク */
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        fprintf(stderr, "Link error.\n");
        return -1;
    }

    return shader;
}

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

    //FragmentShaderをバインド
    glBindFragDataLocation(program, 0, "FragColor");

    //インデックス0をシェーダーの入力変数VertexPositionにBind
    glBindAttribLocation(program, 0, "VertexPosition");

    //インデックス1をシェーダーの入力変数VertexColorにバインド
    glBindAttribLocation(program, 1, "VertexColor");

    //頂点配列オブジェクトへのハンドルを保持するグローバル変数を作成
    GLuint vaoHandle;

    //初期化関数の中で、属性ごとに頂点バッファオブジェクトを作成してデータを投入
    float positionData[] = {
        -0.8f,-0.8f,0.0f,
        0.8f,-0.8f,0.0f,
        0.0f,0.8f,0.0f
    };
    float colorData[] = {
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f
    };

    //バッファオブジェクトを作成
    GLuint vboHandle[2];
    glGenBuffers(2, vboHandle);
    GLuint positionBufferHandle = vboHandle[0];
    GLuint colorBufferHandle = vboHandle[1];

    //位置バッファにデータを投入
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

    //色バッファにデータを投入
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

    //頂点配列オブジェクトの作成
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    //頂点属性配列を有効にする
    glEnableVertexAttribArray(0); //頂点の位置
    glEnableVertexAttribArray(1); //頂点の色

    //インデックス0を位置バッファに対応づける
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

    //インデックス1を色バッファに対応付ける
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

    //アクティブな属性の和人、それらの名前の最大長を取り出す
    GLint maxLength, nAttribs;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &nAttribs);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    //個々の属性名を保持するバッファを割り当てる
    GLchar* name = (GLchar*)malloc(maxLength);

    //個々のアクティブな属性についての情報を取得してプリントする
    GLint written, size, location;
    GLenum type;
    printf("　インデックス  |  名前\n");
    printf("-----------------------------\n");
    for (int i = 0; i < nAttribs; i++)
    {
        glGetActiveAttrib(program, i, maxLength, &written, &size, &type, name);
        location = glGetAttribLocation(program, name);
        printf(" %-5d    |  %s\n", location, name);
    }
    free(name);

    // フレームループ
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        // バッファのクリア
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //頂点配列オブジェクトをバインドしてレンダリングを起動
        //glBindVertexArray(vaoHandle);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //回転した三角形を描画
        SendRotationUniform(&program, 45.0f, &vaoHandle);

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

