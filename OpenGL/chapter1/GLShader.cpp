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
    //�V�F�[�_�[�I�u�W�F�N�g���쐬
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (0 == vertShader || 0 == fragShader)
    {
        fprintf(stderr, "���_�V�F�[�_�[�G���[\n");
        exit(1);
    }

    //�t�@�C���ǂݍ���
    std::string vertShaderStr = readFile(vertex_path);
    std::string fragShaderStr = readFile(fragment_path);
    const char* vertSharderSrc = vertShaderStr.c_str();
    const char* fragSharderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    //Compile vertex Shader
    std::cout << "Compiling vertex shader." << std::endl;
    //�\�[�X�R�[�h����V�F�[�_�[�I�u�W�F�N�g�ɃR�s�[
    glShaderSource(vertShader, 1, &vertSharderSrc, NULL);
    //�V�F�[�_�[���R���p�C��
    glCompileShader(vertShader);

    //�R���p�C���X�e�[�^�X������
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

    //ShaderProgram�������N
    //�v���O�����I�u�W�F�N�g���쐬
    std::cout << "Linking program" << std::endl;
    GLuint program = glCreateProgram();
    //�V�F�[�_�[���v���O�����I�u�W�F�N�g�ɃA�^�b�`
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    //�v���O�����������N
    glLinkProgram(program);

    //�X�e�[�^�X������
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError((logLength > 1) ? logLength : 1);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}