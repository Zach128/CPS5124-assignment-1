#include "utils/vec.hpp"

#include "sphere.hpp"

bool Sphere::ray_intersect(const vec3f &, const vec3f &, float &) const {
    std::cout << "Sphere!" << std::endl;

    // vec3f L = position - orig;
    // float tca = L * dir;
    // float d2 = L * L - tca * tca;

    // if (d2 > radius * radius) return false;

    // float thc = sqrtf(radius * radius - d2);
    // t0 = tca - thc;
    // float t1 = tca + thc;

    // if (t0 < 0) t0 = t1;
    // if (t0 < 0) return false;

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
