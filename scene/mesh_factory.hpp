#pragma once

#include <assetmanager/assetmanager.hpp>
#include <vector>

namespace graphx {

Mesh CreateMesh(std::vector<float> vertices, std::vector<unsigned int> indices);

}
