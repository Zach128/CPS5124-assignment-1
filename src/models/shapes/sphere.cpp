#include "utils/vec.hpp"
#include "renderers/renderer.hpp"

#include "sphere.hpp"

bool Sphere::renderer_ray_intersect(const Renderer &renderer, const vec3f &orig, const vec3f &dir, float &t0) const {
    return renderer.ray_intersect(*this, orig, dir, t0);
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
