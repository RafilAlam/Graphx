#pragma once

#include <deque>
#include <memory>
#include <engine/include/scene/object.hpp>
#include <engine/include/scene/script.hpp>
#include <engine/include/rendering/mesh.hpp>


class Scene {
public:
    Scene() = default;
    
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) noexcept = default;
    Scene& operator=(Scene&&) noexcept = default;

    void AddObject(const Object& mesh);
    template<typename T>
    void AddScript() {
        m_scripts.push_back(
            std::make_unique<T>()
        );
    }

    std::deque<Object>& GetObjects();

    void Update();
private:
    std::deque<Object> m_objects;
    std::deque<std::unique_ptr<Script>> m_scripts;
};