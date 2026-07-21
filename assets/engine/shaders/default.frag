#version 420 core

in vec3 Normal;
in vec2 UV;

out vec4 FragColor;
uniform sampler2D Texture;

layout(std140, binding = 0) 
uniform MaterialData
{
    vec4 Color;
};

void main()
{
    FragColor = texture(Texture, UV);
}