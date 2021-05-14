#pragma once
#include "models/cameras/camera.hpp"

struct PinholeCamera : Camera {
    
    PinholeCamera(const std::string &id, const float fov, const float aspect, const float distance, const vec3f &position, const vec3f &target)
        : Camera(id, "pinhole", fov, aspect, distance, position, target) {}
};
