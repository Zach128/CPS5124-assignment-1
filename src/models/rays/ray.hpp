#pragma once
#include <memory>
#include <limits>

#include "utils/vec.hpp"

class Primitive;

enum RayType { primaryRay, shadowRay };

struct IsectInfo
{
    std::shared_ptr<Primitive> hitObject;
    float tNear = std::numeric_limits<float>::max();
    vec2f uv;
    size_t index = 0;
};

struct RayInfo
{
    vec3f orig;
    vec3f dir;

    RayInfo(const vec3f &orig, const vec3f &dir) : orig(orig), dir(dir) {}
    RayInfo() {}
};
