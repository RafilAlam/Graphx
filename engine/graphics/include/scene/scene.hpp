#pragma once

#include <engine/graphics/include/scene/model.hpp>
#include <engine/physics/include/joint.hpp>
#include <engine/graphics/include/scene/script.hpp>
#include <engine/graphics/include/rendering/assetmanager.hpp>
#include <engine/graphics/include/rendering/debugrenderer.hpp>
#include <engine/core/include/utils.hpp>
#include <engine/physics/include/physicsworld.hpp>
#include <memory>
#include <deque>

class Scene {
public:
    Scene() = default;
    Scene(AssetManager& assetmanager);
    Scene(AssetManager& assetmanager, std::string filepath);
    
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) noexcept = default;
    Scene& operator=(Scene&&) noexcept = default;

    Model& AddModel(std::string name, std::string filepath, glm::vec3 position);
    Joint& AddJoint(std::string name, std::string type, uint64_t modelAid, std::string objectAname, glm::vec3 localanchorA, uint64_t modelBid, std::string objectBname, glm::vec3 localanchorB, simdjson::dom::object& properties);

    template<typename T>
    void AddScript() {
        m_scripts.push_back(
            std::make_unique<T>()
        );
    }

    std::vector<std::unique_ptr<Joint>> joints;
    std::unordered_map<uint64_t, std::unique_ptr<Model>> models;
    std::vector<Object*> objects;
    AssetManager* assetmanager;
    PhysicsWorld physicsworld;
    bool simulatephysics = true;
    bool rendering = true;

    void OnStart();
    void Update(float deltaTime);
private:
    uint64_t newmodelID = 0;
    std::deque<std::unique_ptr<Script>> m_scripts;
    float m_physicstimestep = 1/120.0f;
};