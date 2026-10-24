#pragma once

#include <engine/graphics/include/project/entity.hpp>
#include <engine/physics/include/joint.hpp>
#include <engine/graphics/include/rendering/assetmanager.hpp>
#include <engine/core/include/utils.hpp>
#include <engine/physics/include/physicsworld.hpp>
#include <filesystem>
#include <memory>
#include <deque>

class Scene {
public:
    Scene() = default;
    Scene(AssetManager* assetmanager);
    Scene(AssetManager* assetmanager, std::string filepath);
    
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) noexcept = default;
    Scene& operator=(Scene&&) noexcept = default;

    //Entity& AddModel(std::string name, std::string filepath, glm::vec3 position);
    //Joint& AddJoint(std::string name, std::string type, uint64_t modelAid, std::string objectAname, glm::vec3 localanchorA, uint64_t modelBid, std::string objectBname, glm::vec3 localanchorB, simdjson::dom::object& properties);

    //TODO: replace with sparse arrays to avoid unused indices
    template<typename T>
    T GetComponent(Entity entity) {
        if constexpr (std::is_same_v<T, MeshRenderer>) {
            return meshrenderers.at(entity.id);
        } if constexpr (std::is_same_v<T, Transform>) {
            return transforms.at(entity.id);
        } if constexpr (std::is_same_v<T, RigidBody>) {
            return rigidbodies.at(entity.id);
        } if constexpr (std::is_same_v<T, Collider>) {
            return colliders.at(entity.id);
        }
    }

    std::vector<Transform> transforms;
    std::vector<MeshRenderer> meshrenderers;
    std::vector<RigidBody> rigidbodies;
    std::vector<Collider> colliders;
    //std::vector<Joint> joints;
    AssetManager* assetmanager;
    //PhysicsWorld physicsworld;
    bool activescene{false};
    bool simulatephysics{true};
    bool rendering{true};

    void OnStart();
    void Update(float deltaTime);
private:
    uint64_t newmodelID = 0;
    float m_physicstimestep = 1/120.0f;
};

class Project {
public:
    Project(AssetManager* assetmanager, std::string projectname);
    Scene& GetActiveScene();
    void LoadProject(std::string projectname);
    void AddScene(std::string filepath);
    void AddScene(Scene& scene);
    AssetManager* assetmanager;
    std::vector<Scene> scenes;
    size_t activescene;
};