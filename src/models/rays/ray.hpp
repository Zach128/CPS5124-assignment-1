#pragma once
#include <memory>
#include <limits>

#include "utils/vec.hpp"

class Primitive;

enum RayType { primaryRay, shadowRay };

struct RayInfo
{
    vec3f orig;
    vec3f dir;

    RayInfo(const vec3f &orig, const vec3f &dir) : orig(orig), dir(dir) {}
    RayInfo() {}
};
