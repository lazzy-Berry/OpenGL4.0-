#version 400

uniform struct LightInfo {
    vec4 Position;
    vec3 Intensity;
} Light;

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

uniform sampler2DShadow ShadowMap;

in vec3 Position;
in vec3 Normal;
in vec4 ShadowCoord;

layout (location = 0) out vec4 FragColor;

vec3 phongModelDiffAndSpec()
{
    vec3 n = Normal;
    if( !gl_FrontFacing ) n = -n;
    vec3 s = normalize(vec3(Light.Position) - Position);
    vec3 v = normalize(-Position.xyz);
    vec3 r = reflect( -s, n );
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = Light.Intensity * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Intensity * Material.Ks *
            pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return diffuse + spec;
}

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

subroutine (RenderPassType)
void shadeWithShadow()
{
    vec3 ambient = Light.Intensity * Material.Ka;
    vec3 diffAndSpec = phongModelDiffAndSpec();

    //近くのテクセルの比較の合計
    float sum = 0;

    //ShadowCoordの周りのテクセルからの寄与を合計
    sum += textureProjOffset(ShadowMap,ShadowCoord,ivec2(-1,-1));
    sum += textureProjOffset(ShadowMap,ShadowCoord,ivec2(-1,1));
    sum += textureProjOffset(ShadowMap,ShadowCoord,ivec2(1,1));
    sum += textureProjOffset(ShadowMap,ShadowCoord,ivec2(1,-1));

    float shadow = sum * 0.25;
    FragColor = vec4(ambient + diffAndSpec* shadow,1.0);

    FragColor = pow( FragColor, vec4(1.0 / 2.2) );
}

subroutine(RenderPassType)
void recordDepth()
{}

void main()
{
    RenderPass();
}
