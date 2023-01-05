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

void BindShaders(GLuint* programHandle, GLuint* vaoHandle, GLuint* vboHandle)
{
    //FragmentShader���o�C���h
    glBindFragDataLocation(*programHandle, 0, "FragColor");

    //�C���f�b�N�X0���V�F�[�_�[�̓��͕ϐ�VertexPosition��Bind
    glBindAttribLocation(*programHandle, 0, "VertexPosition");

    //�C���f�b�N�X1���V�F�[�_�[�̓��͕ϐ�VertexColor�Ƀo�C���h
    glBindAttribLocation(*programHandle, 1, "VertexColor");

    //�������֐��̒��ŁA�������Ƃɒ��_�o�b�t�@�I�u�W�F�N�g���쐬���ăf�[�^�𓊓�
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

    //�ʒu�o�b�t�@�Ƀf�[�^�𓊓�
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

    //�F�o�b�t�@�Ƀf�[�^�𓊓�
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

    //���_�z��I�u�W�F�N�g�̍쐬
    glGenVertexArrays(1, vaoHandle);
    glBindVertexArray(*vaoHandle);

    //���_�����z���L���ɂ���
    glEnableVertexAttribArray(0); //���_�̈ʒu
    glEnableVertexAttribArray(1); //���_�̐F

    //�C���f�b�N�X0���ʒu�o�b�t�@�ɑΉ��Â���
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

    //�C���f�b�N�X1��F�o�b�t�@�ɑΉ��t����
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
    //�A�N�e�B�u�ȑ����̘a�l�A�����̖��O�̍ő咷�����o��
    GLint maxLength, nAttribs;
    glGetProgramiv(*programHandle, GL_ACTIVE_ATTRIBUTES, &nAttribs);
    glGetProgramiv(*programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    //�X�̑�������ێ�����o�b�t�@�����蓖�Ă�
    GLchar* name = (GLchar*)malloc(maxLength);

    //�X�̃A�N�e�B�u�ȑ����ɂ��Ă̏����擾���ăv�����g����
    GLint written, size, location;
    GLenum type;
    printf("�@�C���f�b�N�X  |  ���O\n");
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
    //���ׂẴA�N�e�B�u�ȃE�j�t�H�[���ϐ��̖��O�̍ő咷�ƃA�N�e�B�u�ȃ��j�t�H�[���ϐ��̐������o��
    GLint nUniforms, maxLen;

    glGetProgramiv(*programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv(*programHandle, GL_ACTIVE_UNIFORMS, &nUniforms);

    //�e���j�t�H�[���ϐ��̖��O���i�[����X�y�[�X�����蓖�Ă�
    GLchar* name = (GLchar*)malloc(maxLen);

    //�X�̃A�N�e�B�u�ȃ��j�t�H�[���ϐ��̏������o���o��
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

void BindUniformShaders(GLuint* programHandle, GLuint* vaoHandle, GLuint* vboHandle)
{

    //�������֐��̒��ŁA�������Ƃɒ��_�o�b�t�@�I�u�W�F�N�g���쐬���ăf�[�^�𓊓�
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.8f,  0.8f, 0.0f,
         -0.8f, -0.8f, 0.0f,
          0.8f, 0.8f, 0.0f,
          -0.8f, 0.8f, 0.0f
    };
    float tcData[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    glGenBuffers(2, vboHandle);
    GLuint positionBufferHandle = vboHandle[0];
    GLuint tcBufferHandle = vboHandle[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), tcData, GL_STATIC_DRAW);

    //���_�z��I�u�W�F�N�g�̍쐬
    glGenVertexArrays(1, vaoHandle);
    glBindVertexArray(*vaoHandle);

    //���_�����z���L���ɂ���
    glEnableVertexAttribArray(0); 
    glEnableVertexAttribArray(1); 

    //�C���f�b�N�X0���ʒu�o�b�t�@�ɑΉ��Â���
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

    //�C���f�b�N�X1��F�o�b�t�@�ɑΉ��t����
    glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UseUniformBlockAndBuffer(GLuint* programHandle)
{
    //���j�t�H�[���u���b�N�̃C���f�b�N�X���擾
    GLuint blockIndex = glGetUniformBlockIndex(*programHandle, "BlobSettings");

    //���j�t�H�[���u���b�N�̃f�[�^������o�b�t�@�p�̃X�y�[�X�����蓖�Ă�
    GLint blockSize;
    glGetActiveUniformBlockiv(*programHandle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    GLubyte* blockBuffer = (GLubyte*)malloc(blockSize);

    //�e�ϐ��̃u���b�N���̃I�u�Z�b�g��₢���킹��
    const GLchar* names[] = { "InnerColor", "OuterColor",
        "RadiusInner", "RadiusOuter" };
    GLuint indices[4];
    glGetUniformIndices(*programHandle, 4, names, indices);

    GLint offset[4];
    glGetActiveUniformsiv(*programHandle, 4, indices, GL_UNIFORM_OFFSET, offset);

    //�f�[�^���o�b�t�@���ɓK�؂ȃI�t�Z�b�g�Ŕz�u
    GLfloat outerColor[] = { 0.0f,0.0f,0.0f,0.0f };
    GLfloat innerColor[] = { 1.0f,1.0f,0.75f,1.0f };
    GLfloat innerRadius = 0.25f, outerRadius = 0.45f;

    memcpy(blockBuffer + offset[0], innerColor, 4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[1], outerColor, 4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[2], &innerRadius, 4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[3], &outerRadius, 4 * sizeof(GLfloat));

    //OpenGL�o�b�t�@�I�u�W�F�N�g���쐬�������Ƀf�[�^���R�s�[
    GLuint uboHandle;
    glGenBuffers(1, &uboHandle);
    glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
    glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);

    //�o�b�t�@�I�u�W�F�N�g�����j�t�H�[���Ƀo�C���h
    glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uboHandle);
}

void renderUseUniformBlockAndBuffer(GLuint* vaoHandle)
{
    glBindVertexArray(*vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
