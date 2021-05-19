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

void from_json(const json &j, std::shared_ptr<PinholeCamera> &p) {
  p = std::make_shared<PinholeCamera>();

  j.at("id").get_to(p->id);
  j.at("type").get_to(p->type);
  j.at("fov").get_to(p->fov);
  j.at("aspect").get_to(p->aspect);
  j.at("distance").get_to(p->distance);
  p->position = vec3f(j.at("position").get<std::vector<float>>().data());
  p->target = vec3f(j.at("target").get<std::vector<float>>().data());
}