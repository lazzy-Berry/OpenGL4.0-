#version 400

in vec3 ReflectDir; //反射方向

uniform samplerCube CubeMapTex; //キューブマップ

uniform bool DrawSkyBox; //スカイボックスを描くか

uniform float ReflectFactor; //反射の量
uniform vec4 MaterialColor; //オブジェクトの色合い

layout (location = 0) out vec4 FragColor;

void main() 
{
    //キューブマップテクスチャにアクセス
    vec4 cubeMapColor = texture(CubeMapTex,ReflectDir);
    if(DrawSkyBox)
    {
        FragColor = cubeMapColor;
    }
    else
    {
        FragColor = mix(MaterialColor, cubeMapColor,ReflectFactor);
    }
}
