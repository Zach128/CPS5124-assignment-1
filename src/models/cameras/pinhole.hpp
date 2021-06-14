#pragma once
#include <nlohmann/json.hpp>

#include "models/primitive.hpp"
#include "models/cameras/camera.hpp"

using json = nlohmann::json;

struct PinholeCamera : Camera {
    
    PinholeCamera(const std::string &id, const float fov, const float aspect, const float distance, const vec3f &position, const vec3f &target)
        : Camera(id, "pinhole", fov, aspect, distance, position, target) {}
    
    PinholeCamera() : Camera() {}

    vec3f cast_ray(const vec3f &orig, const vec3f &dir, const std::vector<std::shared_ptr<Primitive>> &primitives);
};

void from_json(const json &j, PinholeCamera &c);
void from_json(const json &j, std::shared_ptr<PinholeCamera> &p);
