#include "utils/vec.hpp"
#include "models/shapes/shape.hpp"
#include "models/shapes/sphere.hpp"
#include "models/cameras/camera.hpp"
#include "models/primitive.hpp"

#include "pinhole.hpp"

vec3f PinholeCamera::cast_ray(const vec3f &orig, const vec3f &dir, const std::vector<std::shared_ptr<Primitive>> &primitives) {
    // std::cout << "Pinhole!" << std::endl;

    float sphere_dist = std::numeric_limits<float>::max();

    if(!primitives[0]->shape->ray_intersect(orig, dir, sphere_dist)) {
        return vec3f(0.2, 0.7, 0.8);
    }

    return vec3f(0.4, 0.4, 0.3);
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
