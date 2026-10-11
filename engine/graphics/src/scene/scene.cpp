#include <engine/graphics/include/scene/scene.hpp>

Scene::Scene(AssetManager& assetmanager, std::string filepath) {
    std::string json = LoadTextFile(filepath);
    simdjson::dom::parser parser;
    
    using namespace simdjson::builtin;
    try {
        simdjson::dom::element doc = parser.parse(json);

        simdjson::dom::array objects = doc["objects"];

        for (auto object: objects) {
            std::string_view name = object["name"];
            std::string_view meshpath = object["mesh"];
            std::string_view materialpath = object["material"];
            double mass;
            double x{0.0};
            double y{0.0};
            double z{0.0};
            std::string_view collidertype;
            simdjson::dom::object rigidbody = object["rigid_body"];
            mass = rigidbody["mass"];
            simdjson::dom::object collider = object["collider"];
            collidertype = collider["type"];
            if (collidertype == "rectangle") {
                simdjson::dom::object halfextents = collider["half_extents"];
                x = halfextents["x"];
                y = halfextents["y"];
                z = halfextents["z"];

                const Mesh& mesh = assetmanager.LoadMesh(std::string(meshpath));
                const Material& material = assetmanager.LoadMaterial(std::string(materialpath));
                Collider collider{ColliderType::Polygon, PolygonData::Rectangle()};
                RigidBody rigidbody{mass, {x, y, z}, material.friction, material.restitution};
                Object& object = CreateObject(std::string(name), mesh, material, rigidbody, collider);
                object.transform.scale = {x, y, z};

            } else if (collidertype == "circle") {
                x = collider["radius"];
                const Mesh& mesh = assetmanager.LoadMesh(std::string(meshpath));
                const Material& material = assetmanager.LoadMaterial(std::string(materialpath));
                Collider collider{ColliderType::Circle, CircleData{.radius = x}};
                RigidBody rigidbody{mass, {x, y, z}, material.friction, material.restitution};
                CreateObject(std::string(name), mesh, material, rigidbody, collider);
            }
        }
    } catch (const simdjson::simdjson_error& e) {
        std::cerr << "JSON Parsing failed: " << e.what() << std::endl;
    }
}

Object& Scene::CreateObject(std::string name, const Mesh& mesh, const Material& material) {
    return objects.emplace_back(name, mesh, material);
}

Object& Scene::CreateObject(std::string name, const Mesh& mesh, const Material& material, RigidBody& rigidbody) {
    return objects.emplace_back(name, mesh, material, rigidbody);
}

Object& Scene::CreateObject(std::string name, const Mesh& mesh, const Material& material, RigidBody& rigidbody, Collider& collider) {
    return objects.emplace_back(name, mesh, material, rigidbody, collider);
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
    if (simulatephysics)
        physicsworld.Step(objects, m_physicstimestep);

    for (auto& object : objects) {
        if (object.rigidbody != nullptr) {
            object.transform.position = object.rigidbody->position;
            object.transform.rotation = object.rigidbody->rotation;
        }
    }
}