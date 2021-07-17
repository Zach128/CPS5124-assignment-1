#pragma once
#include <nlohmann/json.hpp>

#include "models/rays/ray.hpp"
#include "models/primitive.hpp"
#include "models/cameras/camera.hpp"

using json = nlohmann::json;

struct PinholeCamera : Camera {

    PinholeCamera(const std::string &id, const float fov, const float aspect, const float distance, const vec3f &position, const vec3f &target)
        : Camera(id, "pinhole", fov, aspect, distance, position, target) {}

    PinholeCamera() : Camera() {}

    vec3f renderer_cast_ray(Renderer &renderer, const RayInfo &ray, float &dist, size_t depth);
};

void from_json(const json &j, PinholeCamera &c);
void from_json(const json &j, std::shared_ptr<PinholeCamera> &p);
