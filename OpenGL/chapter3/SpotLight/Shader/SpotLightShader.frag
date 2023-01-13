#version 400

in vec3 Position;
in vec3 Normal;

struct SpotLightInfo {
    vec4 position; //���_���W�̈ʒu
    vec3 intensity; //�A���r�G���g�A�f�B�t���[�Y�A�X�y�L�������x
    vec3 direction; //�X�|�b�g���C�g�̐��K������
    float exponent; //�p�x���Z�w��
    float cutoff; //�J�b�g�I�t�p�x�i0�`90�j
};
uniform SpotLightInfo Spot;

//�}�e���A���@�p�����[�^
uniform vec3 Ka; //�A���r�G���g���˗�
uniform vec3 Kd; //�f�B�t���[�Y���˗�
uniform vec3 Ks; //�X�y�L�������˗�
uniform float Shininess; //�X�y�L�����P���W��


uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;


layout (location = 0) out vec4 FragColor;

vec3 adsWithSpotlight()
{
    vec3 s = normalize(vec3(Spot.position)-Position);
    vec3 spotDir = normalize(Spot.direction);
    float angle = acos(dot(-s,spotDir));
    float cutoff = radians(clamp(Spot.cutoff,0.0,90.0));
    vec3 ambient = Spot.intensity *Ka;

    if(angle < cutoff)
    {
        float spotFactor = pow(dot(-s,spotDir),Spot.exponent);
        vec3 v = normalize(vec3(-Position));
        vec3 h = normalize(v + s);

        return ambient + spotFactor * Spot.intensity * (Kd * max(dot(s,Normal),0.0)+  Ks * pow(max(dot(h,Normal),0.0),Shininess));
    }
    return ambient;
}

void main()
{
    FragColor = vec4(adsWithSpotlight(), 1.0);
}