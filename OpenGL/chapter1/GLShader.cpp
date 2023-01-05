#include "GLShader.hpp"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::string readFile(const char* filePath) 
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint LoadShader(const char* vertex_path, const char* fragment_path) 
{
    //シェーダーオブジェクトを作成
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (0 == vertShader || 0 == fragShader)
    {
        fprintf(stderr, "頂点シェーダーエラー\n");
        exit(1);
    }

    //ファイル読み込み
    std::string vertShaderStr = readFile(vertex_path);
    std::string fragShaderStr = readFile(fragment_path);
    const char* vertSharderSrc = vertShaderStr.c_str();
    const char* fragSharderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    //Compile vertex Shader
    std::cout << "Compiling vertex shader." << std::endl;
    //ソースコードからシェーダーオブジェクトにコピー
    glShaderSource(vertShader, 1, &vertSharderSrc, NULL);
    //シェーダーをコンパイル
    glCompileShader(vertShader);

    //コンパイルステータスを懸賞
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
    std::cout << &vertShaderError[0] << std::endl;

    //Compile fragment Shader
    std::cout << "Compiling fragment shader." << std::endl;
    glShaderSource(fragShader, 1, &fragSharderSrc, NULL);
    glCompileShader(fragShader);

    //Check vertex shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
    std::cout << &fragShaderError[0] << std::endl;

    //ShaderProgramをリンク
    //プログラムオブジェクトを作成
    std::cout << "Linking program" << std::endl;
    GLuint program = glCreateProgram();
    //シェーダーをプログラムオブジェクトにアタッチ
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    //プログラムをリンク
    glLinkProgram(program);

    //ステータスを検証
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError((logLength > 1) ? logLength : 1);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}

void BindShaders(GLuint* programHandle, GLuint* vaoHandle, GLuint* vboHandle)
{
    //FragmentShaderをバインド
    glBindFragDataLocation(*programHandle, 0, "FragColor");

    //インデックス0をシェーダーの入力変数VertexPositionにBind
    glBindAttribLocation(*programHandle, 0, "VertexPosition");

    //インデックス1をシェーダーの入力変数VertexColorにバインド
    glBindAttribLocation(*programHandle, 1, "VertexColor");

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
    glGenVertexArrays(1, vaoHandle);
    glBindVertexArray(*vaoHandle);

    //頂点属性配列を有効にする
    glEnableVertexAttribArray(0); //頂点の位置
    glEnableVertexAttribArray(1); //頂点の色

    //インデックス0を位置バッファに対応づける
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

    //インデックス1を色バッファに対応付ける
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
}

void SendRotationUniform(GLuint* programHandle, float angle, GLuint* vaoHandle)
{
    glClear(GL_COLOR_BUFFER_BIT);
    mat4 rotationMatrix = glm::rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));
        
    GLuint location = glGetUniformLocation(*programHandle, "RotationMatrix");

    if (location >= 0)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
    }
    glBindVertexArray(*vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void PrintActibAttribs(GLuint* programHandle)
{
    //アクティブな属性の和人、それらの名前の最大長を取り出す
    GLint maxLength, nAttribs;
    glGetProgramiv(*programHandle, GL_ACTIVE_ATTRIBUTES, &nAttribs);
    glGetProgramiv(*programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    //個々の属性名を保持するバッファを割り当てる
    GLchar* name = (GLchar*)malloc(maxLength);

    //個々のアクティブな属性についての情報を取得してプリントする
    GLint written, size, location;
    GLenum type;
    printf("　インデックス  |  名前\n");
    printf("-----------------------------\n");
    for (int i = 0; i < nAttribs; i++)
    {
        glGetActiveAttrib(*programHandle, i, maxLength, &written, &size, &type, name);
        location = glGetAttribLocation(*programHandle, name);
        printf(" %-5d    |  %s\n", location, name);
    }
    free(name);
}

void PrintActiveUniformAttribs(GLuint* programHandle)
{
    //すべてのアクティブなウニフォーム変数の名前の最大長とアクティブなユニフォーム変数の数を取り出す
    GLint nUniforms, maxLen;

    glGetProgramiv(*programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv(*programHandle, GL_ACTIVE_UNIFORMS, &nUniforms);

    //各ユニフォーム変数の名前を格納するスペースを割り当てる
    GLchar* name = (GLchar*)malloc(maxLen);

    //個々のアクティブなユニフォーム変数の情報を取り出し出力
    GLint size, location;
    GLsizei written;
    GLenum type;
    printf(" Location  |  Name \n");
    printf("---------------------------\n");
    for (int i = 0; i < nUniforms; ++i)
    {
        glGetActiveUniform(*programHandle, i, maxLen, &written, &size, &type, name);
        location = glGetUniformLocation(*programHandle, name);
        printf(" %-8d   | %s\n", location, name);
    }
    free(name);
}
