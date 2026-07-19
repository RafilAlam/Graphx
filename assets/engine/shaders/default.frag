#version 420 core

out vec4 FragColor;

layout(std140, binding = 0) 
uniform MaterialData
{
    vec4 Color;
};

void main()
{
    FragColor = Color;
}