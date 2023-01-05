#version 400

//レイアウト修飾子
//属性インデックスを定義することで、glBindAttribLocationを呼び出す必要がなくなる
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec3 Color;

uniform mat4 RotationMatrix;

void main()
{
    Color = VertexColor;
    gl_Position = RotationMatrix * vec4(VertexPosition, 1.0);
}