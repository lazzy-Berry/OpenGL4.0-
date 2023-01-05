#ifndef GLSHADER_H
#define GLSHADER_H

#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

//Shader�t�@�C����ǂݍ���
GLuint LoadShader(const char* vertex_path, const char* fragment_path);

//�V�F�[�_���o�C���h����
void BindShaders(GLuint* programHandle, GLuint* vaoHandle, GLuint* vboHandle);

//���j�t�H�[���ϐ����g���ăV�F�[�_�[�Ƀf�[�^�𑗂�
//�O�p�`����]
void SendRotationUniform(GLuint* programHandle, float angle, GLuint* vaoHandle);

//�X�̃A�N�e�B�u�ȑ����ɂ��Ă̏����擾���ăv�����g����
void PrintActibAttribs(GLuint* programHandle);

//�A�N�e�B�u�ȃ��j�t�H�[���ϐ��̃��X�g�𓾂�
void PrintActiveUniformAttribs(GLuint* programHandle);



#endif