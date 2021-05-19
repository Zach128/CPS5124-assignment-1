#pragma once
#include <iostream>
#include "utils/vec.hpp"
#include "models/object.hpp"

using json = nlohmann::json;

struct Shape : TypedElement {
    vec3f position;

    Shape(const std::string &id, const std::string &type, const vec3f &position) : TypedElement(id, type), position(position) {}

    Shape() {}
};

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
