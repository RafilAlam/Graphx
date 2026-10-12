#include <engine/graphics/include/scene/scene.hpp>

Scene::Scene(AssetManager& assetmanager, std::string filepath) {
    std::string scenejson = LoadTextFile(filepath);
    simdjson::dom::parser sceneparser;
    
    using namespace simdjson::builtin;
    try {
        simdjson::dom::element scenedoc = sceneparser.parse(scenejson);

        simdjson::dom::array models = scenedoc["models"];

        for (auto model : models) {
            std::string modelname = std::string(model["name"]);
            std::string filepath = std::string(model["filepath"]);
            simdjson::dom::object position = model["position"];
            glm::vec3 positionvec = {static_cast<float>(position["x"].get_double()),
                                     static_cast<float>(position["y"].get_double()),
                                     static_cast<float>(position["z"].get_double())};
            
            AddModel(assetmanager, modelname, filepath, positionvec);
        }
    } catch (const simdjson::simdjson_error& e) {
        std::cerr << "JSON Parsing failed: " << filepath << e.what() << std::endl;
    }
}

Model& Scene::AddModel(AssetManager& assetmanager, std::string name, std::string filepath, glm::vec3 position) {
    std::string modeljson = LoadTextFile(filepath);
    simdjson::dom::parser modelparser;
    simdjson::dom::element modeldoc = modelparser.parse(modeljson);
    simdjson::dom::array jsonobjects = modeldoc["objects"];
    
    Model& newmodel = models.try_emplace(name, name).first->second;
    for (auto object : jsonobjects) {
        std::string objectname = std::string(object["name"]);
        std::string meshpath = std::string(object["mesh"]);
        std::string materialpath = std::string(object["material"]);
        double mass;
        float sizex{0.0};
        float sizey{0.0};
        float sizez{0.0};
        std::string_view collidertype;
        simdjson::dom::object rigidbody = object["rigid_body"];
        mass = rigidbody["mass"];
        simdjson::dom::object collider = object["collider"];
        collidertype = collider["type"];
        if (collidertype == "rectangle") {
            simdjson::dom::object halfextents = collider["half_extents"];
            sizex = static_cast<float>(halfextents["x"].get_double());
            sizey = static_cast<float>(halfextents["y"].get_double());
            sizez = static_cast<float>(halfextents["z"].get_double());

            const Mesh& mesh = assetmanager.LoadMesh(std::string(meshpath));
            const Material& material = assetmanager.LoadMaterial(std::string(materialpath));
            Collider collider{ColliderType::Polygon, PolygonData::Rectangle()};
            RigidBody rigidbody{static_cast<float>(mass), {sizex, sizey, sizez}, material.friction, material.restitution};
            Object object{objectname, mesh, material, rigidbody, collider};
            object.transform.scale = {sizex, sizey, sizez};
            newmodel.objects.emplace(objectname, std::move(object));
            objects.push_back(&newmodel.objects.at(objectname));
        } else if (collidertype == "circle") {
            sizex = static_cast<float>(collider["radius"].get_double());
            const Mesh& mesh = assetmanager.LoadMesh(std::string(meshpath));
            const Material& material = assetmanager.LoadMaterial(std::string(materialpath));
            Collider collider{ColliderType::Circle, CircleData{.radius = sizex}};
            RigidBody rigidbody{static_cast<float>(mass), {sizex, sizey, sizez}, material.friction, material.restitution};
            Object object(objectname, mesh, material, rigidbody, collider);
            object.transform.scale = {sizex, sizey, sizez};
            newmodel.objects.emplace(objectname, std::move(object));
            objects.push_back(&newmodel.objects.at(objectname));
        }
    }
    newmodel.SetPosition(position);

    return newmodel;
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
        physicsworld.Step(models, objects, m_physicstimestep);

    for (auto& model : models) {
        for (auto& object : model.second.objects) { 
            if (object.second.rigidbody != nullptr) {
                object.second.transform.position = object.second.rigidbody->position;
                object.second.transform.rotation = object.second.rigidbody->rotation;
            }
        }
    }
}