#pragma once
#include "utils/vec.hpp"
#include "models/object.hpp"

using json = nlohmann::json;

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
    
    Camera() : TypedElement() {}
};

void from_json(const json &j, Camera &c) {
  j.at("id").get_to(c.id);
  j.at("type").get_to(c.type);
  j.at("fov").get_to(c.fov);
  j.at("aspect").get_to(c.aspect);
  j.at("distance").get_to(c.distance);
  c.position = vec3f(j.at("position").get<std::vector<float>>().data());
  c.target = vec3f(j.at("target").get<std::vector<float>>().data());
}

void from_json(const json &j, std::shared_ptr<Camera> &c) {
  c = std::make_shared<Camera>();

  j.at("id").get_to(c->id);
  j.at("type").get_to(c->type);
  j.at("fov").get_to(c->fov);
  j.at("aspect").get_to(c->aspect);
  j.at("distance").get_to(c->distance);
  c->position = vec3f(j.at("position").get<std::vector<float>>().data());
  c->target = vec3f(j.at("target").get<std::vector<float>>().data());
}
