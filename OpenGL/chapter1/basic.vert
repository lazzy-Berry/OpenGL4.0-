#version 400

//in vec3 VertexPosition;
//in vec3 VertexColor;

//���C�A�E�g�C���q
//�����C���f�b�N�X���`���邱�ƂŁAglBindAttribLocation���Ăяo���K�v���Ȃ��Ȃ�
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec3 Color;

void main()
{
    Color = VertexColor;
    gl_Position = vec4(VertexPosition, 1.0);
}