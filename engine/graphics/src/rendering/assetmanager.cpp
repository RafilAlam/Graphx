#include <engine/graphics/include/rendering/assetmanager.hpp>

Mesh& AssetManager::LoadMesh(std::string name, std::vector<Vertex> vertices, std::vector<Face> faces) {
    if (m_meshes.contains(name)) {
        return m_meshes.at(name);
    } else {
        auto [it, inserted] = m_meshes.try_emplace(name, Mesh(vertices, faces));
        return it->second;
    }
}

Mesh& AssetManager::LoadMesh(std::string filepath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
    aiMesh* mesh = scene->mMeshes[0];

    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<Face> faces(mesh->mNumFaces);

    for (unsigned int i=0; i<mesh->mNumVertices; i++) {
        const aiVector3d& vertex = mesh->mVertices[i];
        const aiVector3d& normal = mesh->mNormals[i];
        glm::vec2 uv;
        if (mesh->HasTextureCoords(0)) {
            uv.x = mesh->mTextureCoords[0][i].x;
            uv.y = mesh->mTextureCoords[0][i].y;
        } else {
            uv = {0.0f, 0.0f};
        }

        vertices[i] = Vertex{
            .Position = {vertex.x, vertex.y, vertex.z},
            .Normal   = {normal.x, normal.y, normal.z},
            .UV       = uv
        };
    }

    for (unsigned int i=0; i<mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        faces[i] = Face{
            .a = face.mIndices[0],
            .b = face.mIndices[1],
            .c = face.mIndices[2],
        };
    }

    std::string name = mesh->mName.C_Str();
    if (m_meshes.contains(name)) {
        DebugPrint(std::string("Cached Mesh reused by Asset Manager: " + name));
        return m_meshes.at(name);
    } else {
        DebugPrint(std::string("Mesh created by Asset Manager: " + name));
        return LoadMesh(mesh->mName.C_Str(), vertices, faces);
    }
}

Shader& AssetManager::LoadShaders(std::string name, std::string vertexsourcepath, std::string fragmentsourcepath) {
    if (m_shaders.contains(name)) {
        return m_shaders.at(name);
    } else {
        auto [it, inserted] = m_shaders.try_emplace(name, Shader(vertexsourcepath, fragmentsourcepath));
        return it->second;
    }
}

Material& AssetManager::LoadMaterial(std::string name, Shader& shader, Texture&& texture, float friction, float restitution) {
    if (m_materials.contains(name)) {
        return m_materials.at(name);
    } else {
        auto [it, inserted] = m_materials.try_emplace(name, Material(shader, texture, friction, restitution));
        return it->second;
    }
}

Material& AssetManager::LoadMaterial(std::string filepath) {
    std::string file = LoadTextFile(filepath);
    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(file));
    ryml::NodeRef root = tree.rootref();
    
    std::string name;
    float friction;
    float restitution;
    std::string texturepath;
    std::string vsourcepath;
    std::string fsourcepath;

    root["Name"] >> name;
    root["Friction"] >> friction;
    root["Restitution"] >> restitution;
    root["Texture"] >> texturepath;
    root["Vertex_Shader"] >> vsourcepath;
    root["Fragment_Shader"] >> fsourcepath;
    Shader& shader = LoadShaders(name, vsourcepath, fsourcepath);

    return LoadMaterial(name, shader, Texture(texturepath.c_str()), friction, restitution);
}