#version 400

layout (location = 0 ) in vec3 VertexPosition;
layout (location = 1 ) in vec3 VertexNormal;

out vec3 Color;

uniform vec4 LightPosition;
uniform vec3 LightIntensity;

//�}�e���A���@�p�����[�^
uniform vec3 Ka; //�A���r�G���g���˗�
uniform vec3 Kd; //�f�B�t���[�Y���˗�
uniform vec3 Ks; //�X�y�L�������˗�
uniform float Shininess; //�X�y�L�����P���W��


uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

vec3 ads(vec4 Position, vec3 norm)
{
    vec3 s;
    if(LightPosition.w == 0.0)
    {   
        s = normalize(vec3(LightPosition));
    }else
    {
        s = normalize(vec3(LightPosition - Position));
    }
    vec3 v = normalize ( vec3(-Position));
    vec3 r = reflect(-s, norm);

    return LightIntensity * (Ka + Kd * max( dot(s,norm), 0.0) + Ks * pow(max(dot(r,v),0.0),Shininess));
}

void main()
{
    vec3 eyeNorm =  normalize( NormalMatrix * VertexNormal);
    vec4 eyePosition = ModelViewMatrix * vec4(VertexPosition,1.0);

    //���C�e�B���O��������]��
    Color = ads(eyePosition, eyeNorm);

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
