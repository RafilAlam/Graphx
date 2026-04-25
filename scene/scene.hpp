#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <assetmanager/assetmanager.hpp>

namespace graphx {

struct Transform {
    glm::vec3 position = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
    glm::vec3 scale = {1, 1, 1};
    glm::mat4 GetMatrix() const;

};

struct Details {
    glm::vec4 color = {0, 0, 0, 0};
};

typedef unsigned int EntityID;

class Scene {
public:
    EntityID CreateEntity();
    template<typename T>
    T Get(EntityID id);
    template<typename T>
    void Set(EntityID id, T object);
    unsigned int GetNumEntities();
private:
    std::unordered_map<EntityID, Transform> transforms;
    std::unordered_map<EntityID, Details> details;
    std::unordered_map<EntityID, Mesh> meshes;
    unsigned int numEntities = {0};
};

}