#include <iostream>
#include <algorithm>
#include <memory>
#include <cmath>

#include "models/shapes/sphere.hpp"
#include "utils/utils.hpp"
#include "models/rays/ray.hpp"
#include "area-light.hpp"

void AreaLight::illuminate(const RayInfo &srcRay, const vec3f &hit, const vec3f &N, vec3f &lightDir, vec3f &lightIntensity, float &distance) const {
    if (shape->type == "sphere") {
        lightDir = (shape->position - hit).normalize();
        vec3f R = reflect(srcRay.dir, N);
        vec3f centerToRay = R * dot(lightDir, R) - lightDir;
        float distToCenter = centerToRay.norm();

        vec3f closestPoint = lightDir + centerToRay * std::clamp(std::dynamic_pointer_cast<Sphere>(shape)->radius / distToCenter, 0.f, 1.f);

        lightDir = (closestPoint - hit).normalize();
        distance = (closestPoint - hit).norm();

        lightIntensity = intensity;
    } else {
        lightDir = (shape->position - hit).normalize();
        distance = (shape->position - hit).norm();

        lightIntensity = intensity;
    }
}

void from_json(const json &j, AreaLight &l) {
  j.at("id").get_to(l.id);
  j.at("type").get_to(l.type);
  j.at("shape").get_to(l.shape_id);
  l.intensity = vec3f(j.at("power").get<std::vector<float>>().data());
}

void from_json(const json &j, std::shared_ptr<AreaLight> &l) {
  l = std::make_shared<AreaLight>();


  j.at("id").get_to(l->id);
  j.at("type").get_to(l->type);
  j.at("shape").get_to(l->shape_id);
  l->intensity = vec3f(j.at("power").get<std::vector<float>>().data());
}
