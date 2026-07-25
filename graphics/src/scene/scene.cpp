#include <graphics/include/scene/scene.hpp>

Object& Scene::CreateObject(const Mesh& mesh, const Material& material) {
    return m_objects.emplace_back(mesh, material);
}

Object& Scene::CreateObject(AssetManager& assetmanager, std::string filepath) {
    std::string file = LoadTextFile(filepath);
    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(file));
    ryml::NodeRef root = tree.rootref();
    
    std::string name;
    std::string meshpath;
    std::string materialpath;

    root["name"] >> name;
    root["mesh"] >> meshpath;
    root["material"] >> materialpath;

    const Mesh& mesh = assetmanager.LoadMesh(meshpath);
    const Material& material = assetmanager.LoadMaterial(materialpath);

    return CreateObject(mesh, material);
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
    }
}