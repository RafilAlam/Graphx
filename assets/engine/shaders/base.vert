#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 Normal;
out vec2 UV;

layout (std140, binding = 1)
uniform ObjectData {
    mat4 Transform;
};

void main()
{
    gl_Position = Transform * vec4(aPos, 1.0);
    Normal = aNormal;
    UV = aUV;
}