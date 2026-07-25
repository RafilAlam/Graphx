#include <graphics/include/scene/scene.hpp>

Scene::Scene(AssetManager& assetmanager, std::string filepath) {
    std::string file = LoadTextFile(filepath);
    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(file));
    ryml::NodeRef root = tree.rootref();
    
    ryml::NodeRef objects;
    std::string name;
    std::string meshpath;
    std::string materialpath;
    std::string collidertype;
    float mass;
    float x;
    float y;
    float z;

    objects = root["Objects"];
    for (auto object: objects) {
        object["Name"] >> name;
        object["Mesh"] >> meshpath;
        object["Material"] >> materialpath;
        object["Mass"] >> mass;
        object["ColliderType"] >> collidertype;
        if (collidertype == "Rectangle") {
            ryml::NodeRef halfextents = object["HalfExtents"];
            halfextents["x"] >> x;
            halfextents["y"] >> y;
            if (halfextents.has_child("z")) {
                halfextents["z"] >> z;
            } else {
                z = 0.0f;
            }

            const Mesh& mesh = assetmanager.LoadMesh(meshpath);
            const Material& material = assetmanager.LoadMaterial(materialpath);
            Collider collider{.type = ColliderType::Rectangle, .rectangle = {.halfExtents = {x, y, z}}};
            RigidBody& rigidbody = physicsworld.CreateRigidBody(mass, collider);
            CreateObject(name, mesh, material, rigidbody);

        } else if (collidertype == "Circle") {
            object["Radius"] >> x;
            const Mesh& mesh = assetmanager.LoadMesh(meshpath);
            const Material& material = assetmanager.LoadMaterial(materialpath);
            Collider collider{.type = ColliderType::Circle, .circle = {.radius = x}};
            RigidBody& rigidbody = physicsworld.CreateRigidBody(mass, collider);
            CreateObject(name, mesh, material, rigidbody);
        }
    }
}

Object& Scene::CreateObject(std::string name, const Mesh& mesh, const Material& material) {
    return objects.emplace_back(name, mesh, material);
}

Object& Scene::CreateObject(std::string name, const Mesh& mesh, const Material& material, RigidBody& rigidbody) {
    return objects.emplace_back(name, mesh, material, rigidbody);
}

void Scene::OnStart() {
    for (auto& script : m_scripts) {
        script->OnStart();
    }
}

void Scene::Update(float deltaTime) {
    for (auto& script : m_scripts) {
        script->OnUpdate();
    }
    physicsworld.Step(deltaTime);
    for (auto& object : objects) {
        if (object.rigidbody != nullptr) {
            object.transform.position = object.rigidbody->position;
        }
    }
}