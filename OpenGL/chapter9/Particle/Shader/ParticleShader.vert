#version 400


//�������x�ƊJ�n����
layout (location = 0) in vec3 VertexInitVel;
layout (location = 1) in float StartTime;

out float Transp; //�p�[�e�B�N���̓����x

uniform float Time; //�A�j���[�V��������
uniform vec3 Gravity = vec3(0.0,-0.05,0.0); //���[���h���W
uniform float ParticleLifetime; //�p�[�e�B�N���̍ő����
uniform mat4 MVP;

void main()
{
    vec3 pos = vec3(0.0);
    Transp = 0.0;

    if(Time > StartTime)
    {
        float t = Time - StartTime;
        if(t < ParticleLifetime)
        {
            pos = VertexInitVel * t + Gravity * t * t;
            Transp = 1.0 -t / ParticleLifetime;
        }
    }
    gl_Position = MVP * vec4(pos,1.0);
   
}
