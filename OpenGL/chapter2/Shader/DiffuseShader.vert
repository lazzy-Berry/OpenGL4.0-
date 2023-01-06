#version 400

layout (location = 0 ) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPosition; //���_���W�̃��C�g�ʒu
uniform vec3 Kd; //�f�B�t���[�Y���˗�
uniform vec3 Ld; //�����̋��x

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP; //���e*���f���r���[

void main()
{
    //�@���ƈʒu�����_���W�ɕϊ�
    vec3 tnorm = normalize (NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);
    vec3 s = normalize(vec3(LightPosition-eyeCoords));

    //�f�B�t���[�Y�V�F�[�f�B���O������
    LightIntensity = Ld*Kd* max(dot(s,tnorm),0.0);
    //�ʒu���N���b�v���W�ɕϊ����Č�ɓn��
    gl_Position = MVP * vec4(VertexPosition,1.0);
}