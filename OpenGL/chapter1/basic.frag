#version 400

//
// shader.frag
//

in vec3 Color;
layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(Color,1.0);
}