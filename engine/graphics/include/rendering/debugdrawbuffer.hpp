#pragma once

#include <vector>
#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/graphics/include/rendering/mesh.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

inline const char* corevshader = R"(
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

inline const char* corefshader = R"(
#version 420 core

in vec3 Color;

out vec4 FragColor;

void main()
{
    FragColor = vec4(Color, 1.0);
}
)";

struct color {
    static constexpr glm::vec3 black{0.0f, 0.0f , 0.0f};
    static constexpr glm::vec3 brown{0.55f, 0.27f, 0.07f};
    static constexpr glm::vec3 red{1.0f, 0.0f , 0.0f};
    static constexpr glm::vec3 orange{1.0f, 0.65f, 0.0f};
    static constexpr glm::vec3 yellow{1.0f, 1.0f, 0.0f};
    static constexpr glm::vec3 green{1.0f, 1.0f , 1.0f};
    static constexpr glm::vec3 blue{0.0f, 0.0f , 1.0f};
    static constexpr glm::vec3 violet{0.5f, 0.0f, 0.5f};
    static constexpr glm::vec3 grey{0.5f, 0.5f, 0.5f};
    static constexpr glm::vec3 white{1.0f, 1.0f , 1.0f};
    static constexpr glm::vec3 gold{0.83f, 0.69f, 0.22f};
    static constexpr glm::vec3 silver{0.75f, 0.75f, 0.75f};
};

class App;
class Renderer;

struct DebugVertex {
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 color{1.0f, 1.0f, 1.0f};
};

class DebugDrawBuffer {
    friend class App;
    friend class Renderer;
public:
    static void AddPoint(const glm::vec2 position, const glm::vec3 color);
    static void AddLine(const glm::vec2 p0, const glm::vec2 p1, const glm::vec3 color);
    static void AddArrow(const glm::vec2 position, const glm::vec2 normal, const glm::vec3 color);
    static void FlushPoints();
    static void FlushLines();
    static bool Empty();

    DebugDrawBuffer(const DebugDrawBuffer&) = delete;
    DebugDrawBuffer& operator=(const DebugDrawBuffer&) = delete;
private:
    static void init();
    static inline Shader m_coreshader;
    
    static inline GLuint m_buffers[2];
    static inline GLuint m_arrays[2];

    static inline std::vector<DebugVertex> m_pointvertices;
    static inline std::vector<DebugVertex> m_linevertices;
};