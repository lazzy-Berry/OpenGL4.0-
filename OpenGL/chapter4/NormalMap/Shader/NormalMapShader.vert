#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light;

out vec3 LightDir;
out vec3 ViewDir;
out vec2 TexCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    //法線と接線を視点空間に変換
    vec3 norm = normalize(NormalMatrix * VertexNormal);
    vec3 tang = normalize(NormalMatrix* vec3(VertexTangent));

    //仮法線を計算
    vec3 binomal = normalize(cross(norm,tang)) * VertexTangent.w;

    //接空間に変換するための行列
    mat3 toObjectLocal = mat3(tang.x,binomal.x,norm.x,
                              tang.y,binomal.y,norm.y,
                              tang.z,binomal.z,norm.z);
   
   //位置を視点座標で得る
   vec3 pos = vec3(ModelViewMatrix* vec4(VertexPosition,1.0));

   //ライト方向と視線方向を接空間に変換
   LightDir = normalize(toObjectLocal* (Light.Position.xyz - pos));
   ViewDir = toObjectLocal* normalize(-pos);

   //テクスチャ座標を渡す
   TexCoord = VertexTexCoord;

   gl_Position = MVP * vec4(VertexPosition,1.0);
}