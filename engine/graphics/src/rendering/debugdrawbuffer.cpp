#include <engine/graphics/include/rendering/debugdrawbuffer.hpp>

void DebugDrawBuffer::init()
{
    m_coreshader = {corevshader, corefshader};

    m_points.reserve(10000);
    
    glPointSize(10.0f);

    glGenVertexArrays(1, &m_vertexarray);
    glBindVertexArray(m_vertexarray);

    glGenBuffers(1, &m_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);

    glBufferData(
        GL_ARRAY_BUFFER,
        100000 * sizeof(DebugVertex),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(DebugVertex), 
        (void*)offsetof(DebugVertex, position)
    );

    glVertexAttribPointer(
        1, 
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(DebugVertex), 
        (void*)offsetof(DebugVertex, color)
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void DebugDrawBuffer::AddPoint(const glm::vec2 position, const glm::vec3 color) {
    m_points.emplace_back(glm::vec3{position, 5.0f}, color);
}

void DebugDrawBuffer::Flush() {
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_points.size() * sizeof(DebugVertex), m_points.data());
    m_points.clear();
}

bool DebugDrawBuffer::Empty() {
    return m_points.empty();
}

/*void DebugDrawBuffer::DrawPoints() {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::orthoLH(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 100.0f);

    int transformLocation = glGetUniformLocation(m_coreshader.GetProgram(), "Transform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(projection * view));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_POINTS, 0, m_points.size());
    m_points.clear();
}

void DebugDrawBuffer::Draw() {
    glBindVertexArray(m_vertexarray);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    glUseProgram(m_coreshader.GetProgram());
    
    DrawPoints();
}*/