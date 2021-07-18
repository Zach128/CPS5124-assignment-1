#include "utils/vec.hpp"
#include "models/shapes/shape.hpp"
#include "models/shapes/sphere.hpp"
#include "models/cameras/camera.hpp"
#include "models/primitives/primitive.hpp"
#include "renderers/renderer.hpp"
#include "models/rays/ray.hpp"

#include "pinhole.hpp"

vec3f PinholeCamera::renderer_cast_ray(Renderer &renderer, const RayInfo &ray, float &dist, size_t depth = 0) {
  return renderer.cast_ray(*this, ray, dist, depth);
}

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
