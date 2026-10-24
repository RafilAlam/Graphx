#include <engine/graphics/include/project/project.hpp>

namespace fs = std::filesystem;
namespace od = simdjson::ondemand;

Scene::Scene(AssetManager* assetmanager): assetmanager(assetmanager) {}

Scene::Scene(AssetManager* assetmanager, std::string filepath): assetmanager(assetmanager) {
    std::string scenejson = LoadTextFile(filepath);
    od::parser sceneparser;
    
    using namespace simdjson::builtin;
    try {
        auto doc = sceneparser.iterate(scenejson);
        activescene = doc["activescene"];
        for (od::object entity : doc["entities"]) {
            od::array components = entity["components"];
            for (od::object component : components) {
                if (component["type"] == "transform")
                    transforms.emplace_back(component);
                else if (component["type"] == "meshrenderer") {
                    meshrenderers.emplace_back(component);
                }
            }
            //AddJoint(jointname, type, modelAid, objectAname, localanchorAvec, modelBid, objectBname, localanchorBvec, properties);
        }
    } catch (const simdjson::simdjson_error& e) {
        std::cerr << "JSON Parsing failed: " << filepath << e.what() << std::endl;
    }
}

void Scene::Update(float deltaTime) {
    if (!simulatephysics)
        return;

    /*physicsworld.Step(models, objects, joints, m_physicstimestep);
    for (uint32_t i = 0; i < transforms.size(); ++i) { 
        Transform& transform = transforms.at(i);
        RigidBody& rigidbody = rigidbodies.at(i);
        transform.position = rigidbody.position;
        transform.rotation = rigidbody.rotation;
    }
    for (auto& joint : joints) {
        joint.visuals->Update();
    }*/
}

Project::Project(AssetManager* assetmanager, std::string projectname): assetmanager(assetmanager) {
    LoadProject(projectname);
}

Scene& Project::GetActiveScene() {
    return scenes.at(activescene);
}

void Project::LoadProject(std::string projectname) {
    fs::path projectpath = fs::current_path() / projectname;
    if (fs::exists(projectpath) && fs::is_directory(projectpath)) {
        for (const auto& entry : fs::directory_iterator(projectpath)) {
            if (fs::is_regular_file(entry.path())) {
                AddScene(entry.path().string());
            }
        }
    }
}

void Project::AddScene(std::string filepath) {
    if (scenes.emplace_back(assetmanager, filepath).activescene)
        activescene = scenes.size() - 1;
}

void Project::AddScene(Scene& scene) {
    scenes.emplace_back(std::move(scene));
}