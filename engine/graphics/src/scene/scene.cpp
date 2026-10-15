#include <engine/graphics/include/scene/scene.hpp>

Scene::Scene(AssetManager& assetmanager): assetmanager(&assetmanager) {};

Scene::Scene(AssetManager& assetmanager, std::string filepath): assetmanager(&assetmanager) {
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
            
            AddModel(modelname, filepath, positionvec);
        }

        simdjson::dom::array joints = scenedoc["joints"];

        for (auto joint : joints) {
            std::string jointname = std::string(joint["name"]);
            std::string type = std::string(joint["type"]);
            simdjson::dom::object bodyA = joint["bodyA"];
            uint64_t modelAid = bodyA["model"];
            std::string objectAname = std::string(bodyA["object"]);
            simdjson::dom::object localanchorA = bodyA["localanchor"];
            glm::vec3 localanchorAvec = {static_cast<float>(localanchorA["x"].get_double()),
                                         static_cast<float>(localanchorA["y"].get_double()),
                                         static_cast<float>(localanchorA["z"].get_double())};

            simdjson::dom::object bodyB = joint["bodyB"];
            uint64_t modelBid = bodyB["model"];
            std::string objectBname = std::string(bodyB["object"]);
            simdjson::dom::object localanchorB = bodyB["localanchor"];
            glm::vec3 localanchorBvec = {static_cast<float>(localanchorB["x"].get_double()),
                                         static_cast<float>(localanchorB["y"].get_double()),
                                         static_cast<float>(localanchorB["z"].get_double())};
            
            simdjson::dom::object properties = joint["properties"];
            
            AddJoint(jointname, type, modelAid, objectAname, localanchorAvec, modelBid, objectBname, localanchorBvec, properties);
        }
    } catch (const simdjson::simdjson_error& e) {
        std::cerr << "JSON Parsing failed: " << filepath << e.what() << std::endl;
    }
}

Model& Scene::AddModel(std::string name, std::string filepath, glm::vec3 position) {
    std::string modeljson = LoadTextFile(filepath);
    simdjson::dom::parser modelparser;
    simdjson::dom::element modeldoc = modelparser.parse(modeljson);
    simdjson::dom::array jsonobjects = modeldoc["objects"];
    
    uint64_t currentmodelID = newmodelID++;
    Model& newmodel = *models.try_emplace(currentmodelID, std::make_unique<Model>(currentmodelID, name)).first->second;
    for (auto object : jsonobjects) {
        std::string objectname = std::string(object["name"]);
        std::string meshpath = std::string(object["mesh"]);
        std::string materialpath = std::string(object["material"]);
        simdjson::dom::object localposition = object["localposition"];
        glm::vec3 localpositionvec = {static_cast<float>(localposition["x"].get_double()),
                                      static_cast<float>(localposition["y"].get_double()),
                                      static_cast<float>(localposition["z"].get_double())};
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

            const StaticMesh& mesh = assetmanager->LoadMesh(std::string(meshpath));
            const Material& material = assetmanager->LoadMaterial(std::string(materialpath));
            Collider collider{ColliderType::Polygon, PolygonData::Rectangle()};
            RigidBody rigidbody{static_cast<float>(mass), {sizex, sizey, sizez}, material.friction, material.restitution};
            Object object{objectname, mesh, material, rigidbody, collider};
            object.rigidbody->position = position + localpositionvec;
            object.transform.scale = {sizex, sizey, sizez};
            newmodel.objects.emplace(objectname, std::make_unique<Object>(std::move(object)));
            objects.push_back(&*newmodel.objects.at(objectname));
        } else if (collidertype == "circle") {
            sizex = static_cast<float>(collider["radius"].get_double());
            const StaticMesh& mesh = assetmanager->LoadMesh(std::string(meshpath));
            const Material& material = assetmanager->LoadMaterial(std::string(materialpath));
            Collider collider{ColliderType::Circle, CircleData{.radius = sizex}};
            RigidBody rigidbody{static_cast<float>(mass), {sizex, sizey, sizez}, material.friction, material.restitution};
            Object object(objectname, mesh, material, rigidbody, collider);
            object.rigidbody->position = position + localpositionvec;
            object.transform.scale = {sizex, sizey, sizez};
            newmodel.objects.emplace(objectname, std::make_unique<Object>(std::move(object)));
            objects.push_back(&*newmodel.objects.at(objectname));
        }
    }
    newmodel.position = position;

    return newmodel;
}

Joint& Scene::AddJoint(std::string name, std::string type, uint64_t modelAid, std::string objectAname, glm::vec3 localanchorA, uint64_t modelBid, std::string objectBname, glm::vec3 localanchorB, simdjson::dom::object& properties) {
    Object* objectA = models.at(modelAid)->objects.at(objectAname).get();
    Object* objectB = models.at(modelBid)->objects.at(objectBname).get();

    if (type == "RodJoint") {
        float length = static_cast<float>(properties["length"].get_double());
        StaticMesh& mesh = assetmanager->LoadMesh("assets/engine/meshes/rectangle.obj");
        Material& material = assetmanager->LoadMaterial("assets/engine/materials/rod.json");

        RodJoint* joint = static_cast<RodJoint*>(joints.emplace_back(std::make_unique<RodJoint>(name, objectA, localanchorA, objectB, localanchorB, length, mesh, material)).get());
        objectA->joints.emplace_back(&*joint);
        objectB->joints.emplace_back(&*joint);
        return *joint;
    } else {
        throw std::runtime_error("Invalid joint type.");
    }
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
    if (!simulatephysics)
        return;

    physicsworld.Step(models, objects, joints, m_physicstimestep);

    for (auto& modelpair : models) {
        Model& model = *modelpair.second;
        for (auto& object : model.objects) { 
            if (object.second->rigidbody != nullptr) {
                object.second->transform.position = object.second->rigidbody->position;
                object.second->transform.rotation = object.second->rigidbody->rotation;
            }
        }
    }
    for (auto& joint : joints) {
        joint->visuals->Update();
    }
}