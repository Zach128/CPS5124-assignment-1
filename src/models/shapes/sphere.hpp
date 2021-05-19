#pragma once
#include <iostream>
#include "utils/vec.hpp"
#include "models/shapes/shape.hpp"

using json = nlohmann::json;

struct Sphere : Shape {
    float radius;

    Sphere(const std::string &id, const std::string &type, const vec3f &position, const float radius) : Shape(id, type, position), radius(radius) {}

    Sphere() {}
};

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
