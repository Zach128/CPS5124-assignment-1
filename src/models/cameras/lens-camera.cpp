#include "utils/vec.hpp"
#include "models/shapes/shape.hpp"
#include "models/shapes/sphere.hpp"
#include "models/cameras/camera.hpp"
#include "models/primitive.hpp"
#include "models/rays/ray.hpp"
#include "renderers/renderer.hpp"

#include "lens-camera.hpp"

vec3f LensCamera::renderer_cast_ray(Renderer &renderer, const RayInfo &ray, float &dist, size_t depth = 0) {
  return renderer.cast_ray(*this, ray, dist, depth);
}

void from_json(const json &j, LensCamera &c) {
  nlohmann::from_json(j, static_cast<Camera &>(c));

  j.at("focal distance").get_to(c.focal_distance);
  j.at("alpha").get_to(c.alpha);
}

void from_json(const json &j, std::shared_ptr<LensCamera> &p) {
  p = std::make_shared<LensCamera>();

  j.at("id").get_to(p->id);
  j.at("type").get_to(p->type);
  j.at("fov").get_to(p->fov);
  j.at("focal distance").get_to(p->focal_distance);
  j.at("alpha").get_to(p->alpha);
  j.at("aspect").get_to(p->aspect);
  j.at("distance").get_to(p->distance);
  p->position = vec3f(j.at("position").get<std::vector<float>>().data());
  p->target = vec3f(j.at("target").get<std::vector<float>>().data());
}
