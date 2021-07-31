#include "utils/vec.hpp"
#include "renderers/renderer.hpp"
#include "models/rays/ray.hpp"

#include "sphere.hpp"

bool Sphere::ray_intersect(const RayInfo &ray, float &t0) const {
    vec3f L = position - ray.orig;
    float sq_radius = radius * radius;
    float tca = dot(L, ray.dir);
    float d2 = dot(L, L) - tca * tca;

    if (d2 > sq_radius) return false;

    float thc = sqrtf(sq_radius - d2);
    t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 < 1e-3) t0 = t1;
    if (t0 < 1e-3) return false;

    return true;
}

void from_json(const json &j, Sphere &s) {
    nlohmann::from_json(j, static_cast<Shape &>(s));

    j.at("radius").get_to(s.radius);
}

void from_json(const json &j, std::shared_ptr<Sphere> &s) {
    s = std::make_shared<Sphere>();

    j.at("id").get_to(s->id);

    j.at("type").get_to(s->type);

    j.at("radius").get_to(s->radius);
    s->position = vec3f(j.at("centre").get<std::vector<float>>().data());
}
