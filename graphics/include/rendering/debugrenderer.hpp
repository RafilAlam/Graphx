#pragma once

#include <vector>
#include <graphics/include/scene/object.hpp>
#include <graphics/include/rendering/uniformbuffer.hpp>
#include <graphics/include/rendering/mesh.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct DebugPoint {
    glm::vec3 position{1.0f, 0.0f, 0.0f};
    glm::vec3 color{1.0f, 1.0f, 1.0f};
};

class DebugRenderer {
public:
    DebugRenderer();
    void AddPoint(DebugPoint point);
    void Draw();
    void DrawPoints();
private:
    Shader m_coreshader;
    GLuint m_vertexbuffer;
    GLuint m_vertexarray;
    std::vector<DebugPoint> m_points;
};