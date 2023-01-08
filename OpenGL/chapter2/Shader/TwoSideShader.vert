#version 400

layout (location = 0 ) in vec3 VertexPosition;
layout (location = 1 ) in vec3 VertexNormal;

out vec3 FrontColor;
out vec3 BackColor;

struct LightInfo {
    vec4 Position; //���_���W�ł̃��C�g�ʒu
    vec3 La; //�A���r�G���g���C�g���x
    vec3 Ld; //�f�B�t���[�Y���C�g���x
    vec3 Ls; //�X�y�L�������C�g���x
};
uniform LightInfo Light;

struct MaterialInfo {
    vec3 Ka; //�A���r�G���g���˗�
    vec3 Kd; //�f�B�t���[�Y���˗�
    vec3 Ks; //�X�y�L�������˗�
    float Shiness; //�X�y�L�����P���W��
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void getEyeSpace( out vec3 norm, out vec4 position)
{
    norm =normalize ( NormalMatrix * VertexNormal);
    position =  ModelViewMatrix * vec4(VertexPosition,1.0);
}

vec3 phongModel (vec4 position, vec3 norm)
{
    vec3 s = normalize(vec3(Light.Position-position));
    vec3 v = normalize( -position.xyz);
    vec3 r = reflect( -s, norm);
    vec3 ambient = Light.La * Material.Ka;
    float sDotN = max ( dot(s,norm),0.0);
    vec3 diffuse = Light.Ld * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0)
    {
        spec = Light.Ls * Material.Ks * pow(max(dot(r,v),0.0), Material.Shiness);
    }
    return ambient + diffuse + spec;
}

void main()
{
    vec3 tnorm = normalize ( NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);

    FrontColor = phongModel(eyeCoords,tnorm);
    BackColor = phongModel(eyeCoords,-tnorm);

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
