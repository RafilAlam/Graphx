#pragma once

#include <engine/core/include/common.hpp>
#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/graphics/include/rendering/texture.hpp>
#include <glm/glm.hpp>

class Material  {
public:
    Material(AssetHandle shaderhandle, AssetHandle texturehandle);
    glm::vec4 basecolor = {1.0f, 1.0f, 1.0f, 1.0f};
    AssetHandle texturehandle;
    AssetHandle shaderhandle;
private:
};