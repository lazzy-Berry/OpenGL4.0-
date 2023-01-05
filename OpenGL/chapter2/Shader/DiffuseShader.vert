#version 400

layout (location = 0 ) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPosition; //視点座標のライト位置
uniform vec3 Kd; //ディフューズ反射率
uniform vec3 Ld; //光源の強度

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP; //投影*モデルビュー

void main()
{
    //法線と位置を視点座標に変換
    vec3 tnorm = normalize (NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);
    vec3 s = normalize(vec3(LightPosition-eyeCoords));

    //ディフューズシェーディング方程式
    LightIntensity = Ld*Kd* max(dot(s,tnorm),0.0);
    //位置をクリップ座標に変換して後に渡す
    gl_Position = MVP * vec4(VertexPosition,1.0);
}