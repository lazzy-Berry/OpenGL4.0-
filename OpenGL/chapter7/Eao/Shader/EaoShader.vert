#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 TexCoord0;

out vec3 Normal;
out vec3 Position;
out vec2 TexCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform mat4 ShadowMatrix;

void main()
{
    Position = vec3(ModelViewMatrix * vec4(VertexPosition,1.0));
    Normal = normalize(NormalMatrix* VertexNormal);
    TexCoord = TexCoord0;
    gl_Position = MVP * vec4(VertexPosition,1.0);
}