#pragma once
#include <nlohmann/json.hpp>
#include "models/cameras/camera.hpp"

using json = nlohmann::json;

struct PinholeCamera : Camera {
    
    PinholeCamera(const std::string &id, const float fov, const float aspect, const float distance, const vec3f &position, const vec3f &target)
        : Camera(id, "pinhole", fov, aspect, distance, position, target) {}
    
    PinholeCamera() : Camera() {}
};

void from_json(const json &j, PinholeCamera &c) {
  nlohmann::from_json(j, static_cast<Camera &>(c));
}