#version 400

in vec3 Position;
in vec3 Normal;

struct LightInfo{
    vec4 position;
    vec3 intensity;
};
uniform LightInfo Light;

struct FogInfo{
    float maxDist;
    float minDist;
    vec3 color;
};
uniform FogInfo Fog;

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

layout (location = 0) out vec4 FragColor;

vec3 ads()
{
    vec3 s = normalize(Light.position.xyz - Position.xyz);
    vec3 v = normalize(vec3(-Position));
    vec3 h = normalize(v + s);
    vec3 ambient = Ka * Light.intensity;
    vec3 diffuse = Light.intensity * Kd * max(0.0,dot(s,Normal));
    vec3 spec = Light.intensity * Ks * pow(max(0.0,dot(h,Normal)),Shininess);

    return ambient + diffuse + spec;
}

void main()
{   
    float dist = abs(Position.z);
    float fogFactor = (Fog.maxDist - dist)/ (Fog.maxDist - Fog.minDist);
    fogFactor = clamp(fogFactor,0.0,1.0);
    vec3 shadeColor = ads();
    vec3 color = mix(Fog.color,shadeColor,fogFactor);

    FragColor = vec4(color,1.0);
}