#version 420 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 1)
uniform ObjectData {
    mat4 Transform;
};

void main()
{
    gl_Position = Transform * vec4(aPos, 1.0);
}