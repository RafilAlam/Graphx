#include <scene/scene.hpp>
#include <utilities/utils.hpp>

namespace graphx {

EntityID Scene::CreateEntity() {
    EntityID id = numEntities++;
    transforms[id] = {};
    details[id] = {};
    return id;
}

template<> Transform Scene::Get<Transform>(EntityID id) {
    auto transform = transforms.find(id);
    if (transform != transforms.end()) Error("Scene unable to retrieve Transform of Entity, " + id);
    return transform->second;
}

template<> Details Scene::Get<Details>(EntityID id) {
    auto detail = details.find(id);
    if (detail != details.end()) Error("Scene unable to retrieve Detail of Entity, " + id);
    return detail->second;
}

template<> Mesh Scene::Get<Mesh>(EntityID id) {
    auto mesh = meshes.find(id);
    if (mesh != meshes.end()) Error("Scene unable to retrieve Mesh of Entity, " + id);
    return mesh->second;
}

template<> void Scene::Set<Transform>(EntityID id, Transform object) {
    transforms[id] = object;
}

template<> void Scene::Set<Details>(EntityID id, Details object) {
    details[id] = object;
}

template<> void Scene::Set<Mesh>(EntityID id, Mesh object) {
    meshes[id] = object;
}

unsigned int Scene::GetNumEntities() {
    return numEntities;
}

}