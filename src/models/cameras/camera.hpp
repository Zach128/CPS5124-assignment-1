#pragma once
#include "utils/vec.hpp"
#include "models/object.hpp"

struct Camera : TypedElement {
    float fov;      // Field of View
    float aspect;   // Aspect ratio
    float distance; // Distance from position to target plane.

    vec3f position; // Position of camera.
    vec3f target;   // Looking direction.
    
    Camera(const std::string &id, const std::string &type, const float fov, const float aspect, const float distance, const vec3f &position, const vec3f &target)
        : TypedElement(id, type),
          fov(fov),
          aspect(aspect),
          distance(distance),
          position(position),
          target(target) {}
};
