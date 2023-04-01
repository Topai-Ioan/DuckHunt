#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = true);
    Mesh* CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner, float lungime, float latime, glm::vec3 color, bool fill = true);
    Mesh* CreateTriangle(const std::string &name, glm::vec3 leftBottomCorner, float base, float height, glm::vec3 color, bool fill = true);
    Mesh* Create90Triangle(const std::string &name, glm::vec3 leftBottomCorner, float base, glm::vec3 color, bool fill = true);
    Mesh* CreateCircle(const std::string& name, int circleLines , glm::vec3 color, bool fill = true);
}
