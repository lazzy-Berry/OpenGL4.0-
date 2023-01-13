#version 400

in vec3 Position;
in vec3 Normal;

struct SpotLightInfo {
    vec4 position; //視点座標の位置
    vec3 intensity; //アンビエント、ディフューズ、スペキュラ強度
    vec3 direction; //スポットライトの正規化方向
    float exponent; //角度減算指数
    float cutoff; //カットオフ角度（0～90）
};
uniform SpotLightInfo Spot;

//マテリアル　パラメータ
uniform vec3 Ka; //アンビエント反射率
uniform vec3 Kd; //ディフューズ反射率
uniform vec3 Ks; //スペキュラ反射率
uniform float Shininess; //スペキュラ輝き係数


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