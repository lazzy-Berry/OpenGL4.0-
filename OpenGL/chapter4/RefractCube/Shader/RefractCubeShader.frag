#version 400

in vec3 ReflectDir; //���˕���
in vec3 RefractDir;

uniform samplerCube CubeMapTex; //�L���[�u�}�b�v

uniform bool DrawSkyBox; //�X�J�C�{�b�N�X��`����

struct MaterialInfo{
    float Eta; //���ܗ��̔�
    float ReflectionFactor; //���ˌ��̊���
};
uniform MaterialInfo Material;

layout (location = 0) out vec4 FragColor;

void main() 
{
    //�L���[�u�}�b�v�e�N�X�`���ɃA�N�Z�X
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
