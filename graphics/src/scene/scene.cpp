#include <graphics/include/scene/scene.hpp>

Object& Scene::CreateObject(const Mesh& mesh, const Material& material) {
    return m_objects.emplace_back(mesh, material);
}

std::deque<Object>& Scene::GetObjects() {
    return m_objects;
}

void Scene::OnStart() {
    for (auto& script : m_scripts) {
        script->OnStart();
    }
}

void Scene::Update() {
    for (auto& script : m_scripts) {
        script->OnUpdate();
        script->OnRender();
    }
}