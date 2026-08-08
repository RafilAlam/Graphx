#include <engine/graphics/include/rendering/debugrenderer.hpp>

const char* corevshader = R"(
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform mat4 Transform;

void main()
{
    Color = aColor;
    gl_Position = Transform * vec4(aPos, 1.0);
}
)";

const char* corefshader = R"(
#version 420 core

in vec3 Color;

out vec4 FragColor;

void main()
{
    FragColor = vec4(Color, 1.0);
}
)";

DebugRenderer::DebugRenderer()
 : m_coreshader(Shader(corevshader, corefshader))
{
    glPointSize(1.0f);

    glGenBuffers(1, &m_vertexbuffer);
    glGenVertexArrays(1, &m_vertexarray);
}

void DebugRenderer::AddPoint(DebugPoint point) {
    m_points.push_back(point);
}

void DebugRenderer::DrawPoints() {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::orthoLH(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 100.0f);

    int transformLocation = glGetUniformLocation(m_coreshader.GetProgram(), "Transform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(projection * view));

    glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(DebugPoint), m_points.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugPoint), (void*)offsetof(DebugPoint, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugPoint), (void*)offsetof(DebugPoint, color));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_POINTS, 0, m_points.size());
}

void DebugRenderer::Draw() {
    glBindVertexArray(m_vertexarray);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    glUseProgram(m_coreshader.GetProgram());
    
    DrawPoints();
}