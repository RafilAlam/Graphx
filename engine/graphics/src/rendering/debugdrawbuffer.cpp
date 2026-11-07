#include <engine/graphics/include/rendering/debugdrawbuffer.hpp>

void DebugDrawBuffer::init()
{
    m_coreshader = {corevshader, corefshader};

    m_pointvertices.reserve(10000);
    m_linevertices.reserve(10000);
    
    glPointSize(10.0f);
    glLineWidth(5.0f);

    glGenVertexArrays(2, m_arrays);
    glGenBuffers(2, m_buffers);

    // Initialise Point Buffer
    glBindVertexArray(m_arrays[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);

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

    // Initialise Line Buffer
    glBindVertexArray(m_arrays[1]);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);

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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DebugDrawBuffer::AddPoint(const glm::vec2 position, const glm::vec3 color) {
    m_pointvertices.emplace_back(glm::vec3{position, 5.0f}, color);
}

void DebugDrawBuffer::AddLine(const glm::vec2 p0, const glm::vec2 p1, const glm::vec3 color) {
    m_linevertices.emplace_back(glm::vec3{p0, 5.0f}, color);
    m_linevertices.emplace_back(glm::vec3{p1, 5.0f}, color);
}

void DebugDrawBuffer::AddArrow(const glm::vec2 position, const glm::vec2 normal, const glm::vec3 color) {
    glm::vec3 endpoint = glm::vec3{position + normal, 5.0f};
    glm::vec3 wingdisplacement = endpoint - glm::vec3{glm::normalize(normal) * 10.0f, 0.0f};
    glm::vec3 perpendicular = glm::normalize(glm::vec3{-normal.y, normal.x, 0.0f}) * 10.0f;
    AddLine(glm::vec3{position, 5.0f}, endpoint, color);
    AddLine(endpoint, wingdisplacement + perpendicular, color);
    AddLine(endpoint, wingdisplacement - perpendicular, color);
}

void DebugDrawBuffer::FlushPoints() {
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_pointvertices.size() * sizeof(DebugVertex), m_pointvertices.data());
    m_pointvertices.clear();
}

void DebugDrawBuffer::FlushLines() {
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_linevertices.size() * sizeof(DebugVertex), m_linevertices.data());
    m_linevertices.clear();
}

bool DebugDrawBuffer::Empty() {
    return m_pointvertices.empty() && m_linevertices.empty();
}

/*void DebugDrawBuffer::DrawPoints() {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::orthoLH(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 100.0f);

    int transformLocation = glGetUniformLocation(m_coreshader.GetProgram(), "Transform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(projection * view));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_POINTS, 0, m_pointvertices.size());
    m_pointvertices.clear();
}

void DebugDrawBuffer::Draw() {
    glBindVertexArray(m_vertexarray);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    glUseProgram(m_coreshader.GetProgram());
    
    DrawPoints();
}*/