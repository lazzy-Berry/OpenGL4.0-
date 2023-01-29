#version 400


//初期速度と開始時刻
layout (location = 0) in vec3 VertexInitVel;
layout (location = 1) in float StartTime;

out float Transp; //パーティクルの透明度

uniform float Time; //アニメーション時刻
uniform vec3 Gravity = vec3(0.0,-0.05,0.0); //ワールド座標
uniform float ParticleLifetime; //パーティクルの最大寿命
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
