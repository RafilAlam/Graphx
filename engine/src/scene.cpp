#include <engine/include/scene.hpp>

void Scene::AddMesh(Mesh mesh) {
    m_meshes.push_back(mesh);
}

void Scene::Update() {
    for (auto& script : m_scripts) {
        script->OnUpdate();
        script->OnRender();
    }
}