#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;
uniform sampler2D BlurTex;

uniform int Width;
uniform int Height;
uniform float LumThresh; //輝度閾値

subroutine vec4 RenderPassType();
subroutine uniform RenderPassType RenderPass;

layout (location = 0) out vec4 FragColor;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light[2];

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

//ガウスブラー用の重みとオフセット
uniform float PixOffset[10] = 
    float[](0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0);
uniform float Weight[10];

vec3 phongModel( vec3 pos, vec3 norm ,int lightIdx)
{
    vec3 s = normalize(vec3(Light[lightIdx].Position) - pos);
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light[lightIdx].Intensity * Material.Ka;
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light[lightIdx].Intensity * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light[lightIdx].Intensity * Material.Ks *
               pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return ambient + diffuse + spec;
}

//RGB値の明るさを近似
float luma (vec3 color)
{
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

//パス１
subroutine (RenderPassType)
vec4 pass1()
{
    vec4 color = vec4(0.0);
    for( int i = 0; i < 2; i++ ) {
        color += vec4(phongModel( Position, Normal, i ),1.0);
    }
    return color;
}

//パス２
subroutine (RenderPassType)
vec4 pass2()
{
    vec4 val = texture(RenderTex, TexCoord);
    if( luma(val.rgb) > LumThresh)
        return val * 0.5;
    else
        return vec4(0.0);
}

//一つ目のブラーパス
subroutine(RenderPassType)
vec4 pass3()
{
    float dy = 1.0 / float(Height);

    vec4 sum = texture(BlurTex,TexCoord) * Weight[0];
    for(int i = 1;i<10;i++)
    {
        sum += texture(BlurTex,TexCoord + vec2(0.0,PixOffset[i])*dy) *Weight[i];
        sum += texture(BlurTex,TexCoord - vec2(0.0,PixOffset[i])*dy) *Weight[i];
    }
    return sum;
}

//2つ目のブラーとオリジナルへの加算
subroutine(RenderPassType)
vec4 pass4()
{
    float dx = 1.0 / float(Width);

    vec4 val = texture(RenderTex,TexCoord);
    vec4 sum = texture(BlurTex,TexCoord) * Weight[0];
    for(int i = 1;i<10;i++)
    {
        sum += texture(BlurTex,TexCoord + vec2(PixOffset[i],0.0)*dx) *Weight[i];
        sum += texture(BlurTex,TexCoord - vec2(PixOffset[i],0.0)*dx) *Weight[i];
    }
    return val +sum;
}

void main()
{
    FragColor = RenderPass();
}


