#pragma once
#include <memory>
#include <limits>

#include "models/primitive.hpp"
#include "utils/vec.hpp"

enum RayType { primaryRay, shadowRay };

struct IsectInfo
{
    std::shared_ptr<Primitive> hitObject;
    float tNear = std::numeric_limits<float>::max();
    vec2f uv;
    size_t index = 0;
};
