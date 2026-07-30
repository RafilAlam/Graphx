#pragma once

#include <graphics/include/scene/object.hpp>
#include <graphics/include/scene/script.hpp>
#include <graphics/include/rendering/assetmanager.hpp>
#include <graphics/include/utils.hpp>
#include <physics/include/physicsworld.hpp>
#include <ryml_std.hpp>
#include <ryml.hpp>
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

    Object& CreateObject(std::string name, const Mesh& mesh, const Material& material);
    Object& CreateObject(std::string name, const Mesh& mesh, const Material& material, RigidBody& rigidbody);
    Object& CreateObject(std::string name, const Mesh& mesh, const Material& material, RigidBody& rigidbody, Collider& collider);

    template<typename T>
    void AddScript() {
        m_scripts.push_back(
            std::make_unique<T>()
        );
    }

    std::deque<Object> objects;
    PhysicsWorld physicsworld;

    void OnStart();
    void Update(float deltaTime);
private:
    std::deque<std::unique_ptr<Script>> m_scripts;
};