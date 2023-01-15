#version 400

in vec3 ReflectDir; //���˕���

uniform samplerCube CubeMapTex; //�L���[�u�}�b�v

uniform bool DrawSkyBox; //�X�J�C�{�b�N�X��`����

uniform float ReflectFactor; //���˂̗�
uniform vec4 MaterialColor; //�I�u�W�F�N�g�̐F����

layout (location = 0) out vec4 FragColor;

void main() 
{
    //�L���[�u�}�b�v�e�N�X�`���ɃA�N�Z�X
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
