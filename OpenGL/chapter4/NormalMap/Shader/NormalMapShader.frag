#version 400

in vec3 LightDir;
in vec2 TexCoord;
in vec3 ViewDir;

uniform sampler2D ColorTex;
uniform sampler2D NormalMapTex;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light;

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

vec3 phongModel( vec3 norm, vec3 diffB)
{
    vec3 r = reflect(-LightDir,norm);
    vec3 ambient = Light.Intensity * Material.Ka;
    float sDotN = max(dot(LightDir,norm),0.0);
    vec3 diffuse = Light.Intensity * diffB * sDotN;
     
    vec3 spec = vec3(0.0);
    if(sDotN > 0.0)
        spec = Light.Intensity * Material.Ks * pow(max(dot(r,ViewDir),0.0), Material.Shininess);
    return ambient + diffuse + spec;
}

void main() {
    //法線マップから法線を取り出す
    vec4 normal = texture(NormalMapTex, TexCoord);

    //色テクスチャをディフューズ反射率として使う
    vec4 texColor = texture(ColorTex,TexCoord);
    FragColor = vec4( phongModel(normal.xyz, texColor.rgb), 1.0);
}
