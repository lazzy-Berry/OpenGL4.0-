#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 ReflectDir; //???ܕ???
out vec3 RefractDir; //???ߕ???

struct MaterialInfo{
    float Eta; //???ܗ??̔?
    float ReflectionFactor; //???ˌ??̊???
};
uniform MaterialInfo Material;

uniform bool DrawSkyBox; //?X?J?C?{?b?N?X???`????
uniform vec3 WorldCameraPosition;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    if(DrawSkyBox)
    {
        ReflectDir = VertexPosition;
    }
    else
    {
        //???[???h???W?̔??˕??????v?Z
        vec3 worldPos = vec3(ModelMatrix * vec4(VertexPosition,1.0));
        vec3 worldNorm = vec3(ModelMatrix * vec4(VertexNormal,0.0));
        vec3 worldView = normalize(WorldCameraPosition - worldPos);
        ReflectDir = reflect(-worldView,worldNorm);

        ReflectDir = reflect(-worldView,worldNorm);
        RefractDir = refract(-worldView,worldNorm,Material.Eta);
    }
    gl_Position = MVP * vec4(VertexPosition,1.0);
}