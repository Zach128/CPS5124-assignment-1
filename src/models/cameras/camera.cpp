#include "utils/vec.hpp"

#include "camera.hpp"

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
