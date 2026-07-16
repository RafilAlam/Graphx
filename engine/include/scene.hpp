#pragma once

#include <vector>
#include <memory>
#include <engine/include/script.hpp>
#include <engine/include/rendering/mesh.hpp>

class Scene {
public:
    Scene() = default;
    
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) noexcept = default;
    Scene& operator=(Scene&&) noexcept = default;

    void AddMesh(Mesh mesh);
    template<typename T>
    void AddScript() {
        m_scripts.push_back(
            std::make_unique<T>()
        );
    }
    void Update();
private:
    std::vector<Mesh> m_meshes;
    std::vector<std::unique_ptr<Script>> m_scripts;
};