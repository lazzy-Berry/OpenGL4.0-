#version 400

in vec3 ReflectDir; //反射方向
in vec3 RefractDir;

uniform samplerCube CubeMapTex; //キューブマップ

uniform bool DrawSkyBox; //スカイボックスを描くか

struct MaterialInfo{
    float Eta; //屈折率の比
    float ReflectionFactor; //反射光の割合
};
uniform MaterialInfo Material;

layout (location = 0) out vec4 FragColor;

void main() 
{
    //キューブマップテクスチャにアクセス
    vec4 reflectColor = texture(CubeMapTex,ReflectDir);
    vec4 refractColor = texture(CubeMapTex,RefractDir);
    if(DrawSkyBox)
    {
        FragColor = reflectColor;
    }
    else
    {
        FragColor = mix(refractColor,reflectColor,Material.ReflectionFactor);
    }
}
