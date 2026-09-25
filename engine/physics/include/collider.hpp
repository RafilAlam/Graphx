#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <variant>
#include <memory>
#include <vector>

enum class ColliderType {
    Polygon,
    Circle,
    Count
};

struct Projection {
    float min;
    float max;

    float getOverlap(Projection p) {
        float overlapMin = std::max(min, p.min);
        float overlapMax = std::min(max, p.max);

        return overlapMax - overlapMin;
    }
};

struct CircleData {
    float radius;
};

struct PolygonData {
    std::vector<glm::vec3> localvertices;
    std::vector<glm::vec3> worldvertices;

    static PolygonData Rectangle() {
        std::vector<glm::vec3> vertices = {
            {-1.0f, -1.0f, 0.0f},
            {1.0f, -1.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {-1.0f, 1.0f, 0.0f}
        };

        return PolygonData{.localvertices = vertices, .worldvertices = vertices};
    }
};

constexpr int ToIndex(ColliderType type) {
    return static_cast<int>(type);
}

struct Collider {
    ColliderType type;
    std::variant<CircleData, PolygonData> shapedata;
    
    std::vector<std::vector<glm::vec3>> getEdges() {
        const std::vector<glm::vec3>& vertices = std::get<PolygonData>(shapedata).worldvertices;
        std::vector<std::vector<glm::vec3>> faces(vertices.size());
        for (size_t i=0; i<vertices.size(); ++i) {
            glm::vec3 p1 = vertices[i];
            glm::vec3 p2 = vertices[i + 1 == vertices.size() ? 0 : i + 1];
            faces[i] = {p1, p2};
        }
        return faces;
    }

    std::vector<glm::vec3> getNormals() {
        const std::vector<glm::vec3>& vertices = std::get<PolygonData>(shapedata).worldvertices;
        std::vector<glm::vec3> axes(vertices.size());
        for (size_t i=0; i<vertices.size(); ++i) {
            glm::vec3 p1 = vertices[i];
            glm::vec3 p2 = vertices[i + 1 == vertices.size() ? 0 : i + 1];
            glm::vec3 face = p2 - p1;
            face = {face.y, -face.x, 0.0f};
            axes[i] = glm::normalize(face);
        }
        return axes;
    }

    Projection project(const glm::vec3& axis) {
        const std::vector<glm::vec3>& vertices = std::get<PolygonData>(shapedata).worldvertices;
        float min = glm::dot(axis, vertices[0]);
        float max = min;
        for (size_t i = 1; i < vertices.size(); ++i) {
            float p = glm::dot(axis, vertices[i]);
            if (p < min) {
                min = p;
            } else if (p > max) {
                max = p;
            }
        }
        return {min, max};
    }

    std::vector<glm::vec3> getMostAligningFace(glm::vec3 referenceNormal) {
        const std::vector<glm::vec3>& vertices = std::get<PolygonData>(shapedata).worldvertices;
        float maxdot = -FLT_MAX;
        std::vector<glm::vec3> referenceFace(3);
        for (size_t i=0; i < vertices.size(); ++i) {
            glm::vec3 p1 = vertices[i];
            size_t nexti = i+1==vertices.size() ? 0 : i+1;
            glm::vec3 p2 = vertices[nexti];
            glm::vec3 face = p2 - p1;
            glm::vec3 Normal = {face.y, -face.x, 0.0f};
            Normal = glm::normalize(Normal);

            float d = glm::dot(Normal, referenceNormal);

            if (d > maxdot) {
                maxdot = d;
                referenceFace = {p1, p2, {i, nexti, 0.0f}};
            }
        }

        return referenceFace;
    }
};