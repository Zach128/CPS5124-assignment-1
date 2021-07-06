#include <iostream>
#include <memory>

#include "point-light.hpp"

void from_json(const json &j, PointLight &l) {
  j.at("id").get_to(l.id);
  j.at("type").get_to(l.type);
  l.position = vec3f(j.at("position").get<std::vector<float>>().data());
  l.intensity = vec3f(j.at("power").get<std::vector<float>>().data());
}

void from_json(const json &j, std::shared_ptr<PointLight> &l) {
  l = std::make_shared<PointLight>();

  j.at("id").get_to(l->id);
  j.at("type").get_to(l->type);
  l->position = vec3f(j.at("position").get<std::vector<float>>().data());
  l->intensity = vec3f(j.at("power").get<std::vector<float>>().data());
}