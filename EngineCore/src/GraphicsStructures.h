#pragma once

#include "Ray.h"

#include <glm/glm.hpp>

namespace Cicada {

struct MouseParams {
    double x = 0, y = 0;
    double windowX = 0, windowY = 0;
    int windowWidth = 0, windowHeight = 0;
};

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float attenuationCoef;
};

}
