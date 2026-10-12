#pragma once

#include <engine/graphics/include/scene/model.hpp>
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
    Scene(AssetManager& assetmanager, std::string filepath);
    
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) noexcept = default;
    Scene& operator=(Scene&&) noexcept = default;

    Model& AddModel(AssetManager& assetmanager, std::string name, std::string filepath, glm::vec3 position);

    template<typename T>
    void AddScript() {
        m_scripts.push_back(
            std::make_unique<T>()
        );
    }

    std::unordered_map<std::string, Model> models;
    std::vector<Object*> objects;
    PhysicsWorld physicsworld;
    bool simulatephysics = true;
    bool rendering = true;

    void OnStart();
    void Update(float deltaTime);
private:
    std::deque<std::unique_ptr<Script>> m_scripts;
    float m_physicstimestep = 1/120.0f;
};