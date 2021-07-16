#pragma once
#include <nlohmann/json.hpp>

#include "models/primitive.hpp"
#include "models/cameras/camera.hpp"

using json = nlohmann::json;

struct LensCamera : Camera {
    float focal_distance;
    float alpha;

    LensCamera(const std::string &id, const float focal_distance, const float alpha, const float fov, const float aspect, const float distance, const vec3f &position, const vec3f &target)
        : Camera(id, "lens-based", fov, aspect, distance, position, target), focal_distance(focal_distance), alpha(alpha) {}

    LensCamera() : Camera() {}

    vec3f renderer_cast_ray(Renderer &renderer, const vec3f &orig, const vec3f &dir, float &dist, size_t depth);
};

void from_json(const json &j, LensCamera &c);
void from_json(const json &j, std::shared_ptr<LensCamera> &p);
