#include <iostream>
#include <memory>

#include "models/rays/ray.hpp"
#include "point-light.hpp"

// void PointLight::illuminate(const vec3f &P, vec3f &lightDir, vec3f &lightIntensity, float &distance) const {
//     lightDir = (P - position);

//     float r2 = lightDir.norm();
//     distance = sqrt(r2);
//     lightDir.x /= distance, lightDir.y /= distance, lightDir.z /= distance;
//     // avoid division by 0
//     lightIntensity = intensity / (4 * M_PI * r2);
// }

void PointLight::illuminate(const RayInfo &, const vec3f &hit, const vec3f &, vec3f &lightDir, vec3f &lightIntensity, float &distance) const {
    lightDir = (position - hit).normalize();
    distance = (position - hit).norm();

    lightIntensity = intensity;
}

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