#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>

std::string LoadTextFile(std::string path);

float Cross2D(glm::vec3 a, glm::vec3 b);