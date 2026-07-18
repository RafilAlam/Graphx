#include <engine/include/scene/scene.hpp>

void Scene::AddObject(const Object& mesh) {
    m_objects.push_back(mesh);
}

std::deque<Object>& Scene::GetObjects() {
    return m_objects;
}

void Scene::Update() {
    for (auto& script : m_scripts) {
        script->OnUpdate();
        script->OnRender();
    }
}