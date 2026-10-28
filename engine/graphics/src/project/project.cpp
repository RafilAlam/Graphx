#include <engine/graphics/include/project/project.hpp>

namespace fs = std::filesystem;

Scene::Scene(AssetManager* assetmanager): assetmanager(assetmanager) {}

Scene::Scene(AssetManager* assetmanager, std::string filepath): assetmanager(assetmanager) {
    yyjson_doc* doc = yyjson_read_file(filepath.c_str(), 0, nullptr, nullptr);
    yyjson_val* root = yyjson_doc_get_root(doc);
    activescene = yyjson_get_bool(yyjson_obj_get(root, "activescene"));

    yyjson_val* entities = yyjson_obj_get(root, "entities");

    size_t ide, maxe;
    yyjson_val* entity;
    yyjson_arr_foreach(entities, ide, maxe, entity) {
        yyjson_val* components = yyjson_obj_get(entity, "components");

        size_t idc, maxc;
        yyjson_val* component;
        yyjson_arr_foreach(components, idc, maxc, component) {
            std::string_view type = yyjson_get_str(yyjson_obj_get(component, "type"));
            if (type == "transform") {
                yyjson_val* position = yyjson_obj_get(component, "position");
                yyjson_val* half_extents = yyjson_obj_get(component, "half_extents");
                transforms.at(ide) = Transform(position, half_extents);
                DebugPrint("Transform Added");
            } else if (type == "meshrenderer") {
                AssetHandle meshhandle = assetmanager->GetMesh(yyjson_get_str(yyjson_obj_get(component, "mesh")));
                AssetHandle materialhandle = assetmanager->GetMaterial(yyjson_get_str(yyjson_obj_get(component, "material")));
                meshrenderers.at(ide) = MeshRenderer(meshhandle, materialhandle);
                DebugPrint("MeshRenderer Added");
            } else if (type == "rigidbody") {
                float mass = static_cast<float>(yyjson_get_num(yyjson_obj_get(component, "mass")));
                float friction = static_cast<float>(yyjson_get_num(yyjson_obj_get(component, "friction")));
                float restitution = static_cast<float>(yyjson_get_num(yyjson_obj_get(component, "restitution")));
                yyjson_val* half_extents = yyjson_obj_get(component, "half_extents");
                glm::vec3 sizevec{
                    static_cast<float>(yyjson_get_num(yyjson_obj_get(half_extents, "x"))),
                    static_cast<float>(yyjson_get_num(yyjson_obj_get(half_extents, "y"))),
                    static_cast<float>(yyjson_get_num(yyjson_obj_get(half_extents, "z")))
                };
                rigidbodies.at(ide) = RigidBody(mass, sizevec, friction, restitution);
            } else if (type == "collider") {
                colliders.at(ide) = Collider{ColliderType::Polygon, PolygonData::Rectangle()};
            }
        }
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
    DebugPrint("Loading Project: " + projectname);
    fs::path projectpath = fs::current_path() / "projects" / projectname;
    if (fs::exists(projectpath) && fs::is_directory(projectpath)) {
        for (const auto& entry : fs::directory_iterator(projectpath)) {
            if (fs::is_regular_file(entry.path())) {
                AddScene(entry.path().string());
            }
        }
    }
    DebugPrint("Loaded");
}

void Project::AddScene(std::string filepath) {
    DebugPrint("Scene added: " + filepath);
    Scene& scene = scenes.emplace_back(assetmanager, filepath);
    if (scene.activescene) {
        activescene = scenes.size() - 1;
        DebugPrint("Active Scene set to " + filepath);
    }
}

void Project::AddScene(Scene& scene) {
    scenes.emplace_back(std::move(scene));
}