#include "utils/vec.hpp"

#include "models/shapes/shape.hpp"

void from_json(const json &j, Shape &s) {
    j.at("id").get_to(s.id);
    j.at("type").get_to(s.type);
    s.position = vec3f(j.at("centre").get<std::vector<float>>().data());
}

void from_json(const json &j, std::shared_ptr<Shape> &s) {
    s = std::make_shared<Shape>();

    j.at("id").get_to(s->id);
    j.at("type").get_to(s->type);
    s->position = vec3f(j.at("centre").get<std::vector<float>>().data());
}