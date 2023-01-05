#include "GLShader.hpp"

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