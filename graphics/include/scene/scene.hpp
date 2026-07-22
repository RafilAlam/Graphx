#pragma once

#include <deque>
#include <memory>
#include <graphics/include/scene/object.hpp>
#include <graphics/include/scene/script.hpp>
#include <graphics/include/rendering/mesh.hpp>


class Scene {
public:
    Scene() = default;
    
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) noexcept = default;
    Scene& operator=(Scene&&) noexcept = default;

    Object& CreateObject(const Mesh& mesh, const Material& material);
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