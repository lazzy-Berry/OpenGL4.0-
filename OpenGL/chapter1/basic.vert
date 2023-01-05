#version 400

//���C�A�E�g�C���q
//�����C���f�b�N�X���`���邱�ƂŁAglBindAttribLocation���Ăяo���K�v���Ȃ��Ȃ�
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec3 Color;

uniform mat4 RotationMatrix;

void main()
{
    Color = VertexColor;
    gl_Position = RotationMatrix * vec4(VertexPosition, 1.0);
}