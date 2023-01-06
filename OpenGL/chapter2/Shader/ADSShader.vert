#version 400

layout (location = 0 ) in vec3 VertexPosition;
layout (location = 1 ) in vec3 VertexNormal;

out vec3 LightIntensity;

struct LightInfo {
    vec4 Position; //視点座標でのライト位置
    vec3 La; //アンビエントライト強度
    vec3 Ld; //ディフューズライト強度
    vec3 Ls; //スペキュラライト強度
};
uniform LightInfo Light;

struct MaterialInfo {
    vec3 Ka; //アンビエント反射率
    vec3 Kd; //ディフューズ反射率
    vec3 Ks; //スペキュラ反射率
    float Shiness; //スペキュラ輝き係数
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    vec3 tnorm = normalize ( NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);
    vec3 s = normalize(vec3(Light.Position-eyeCoords));
    vec3 v = normalize( -eyeCoords.xyz);
    vec3 r = reflect( -s, tnorm);
    vec3 ambient = Light.La * Material.Ka;
    float sDotN = max ( dot(s,tnorm),0.0);
    vec3 diffuse = Light.Ld * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0)
    {
        spec = Light.Ls * Material.Ks * pow(max(dot(r,v),0.0), Material.Shiness);
    }
    LightIntensity = ambient + diffuse + spec;
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
