#pragma once

#include <graphics/include/scene/object.hpp>
#include <graphics/include/scene/script.hpp>
#include <graphics/include/rendering/assetmanager.hpp>
#include <graphics/include/rendering/mesh.hpp>
#include <graphics/include/utils.hpp>
#include <ryml_std.hpp>
#include <ryml.hpp>
#include <memory>
#include <deque>

class Scene {
public:
    Scene() = default;
    
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) noexcept = default;
    Scene& operator=(Scene&&) noexcept = default;

    Object& CreateObject(const Mesh& mesh, const Material& material);
    Object& CreateObject(AssetManager& assetmanager, std::string filepath);

    template<typename T>
    void AddScript() {
        m_scripts.push_back(
            std::make_unique<T>()
        );
    }

    std::deque<Object>& GetObjects();

    void OnStart();
    void Update();
private:
    std::deque<Object> m_objects;
    std::deque<std::unique_ptr<Script>> m_scripts;
};