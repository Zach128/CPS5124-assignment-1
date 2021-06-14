#pragma once
#include <iostream>
#include "utils/vec.hpp"
#include "models/object.hpp"

using json = nlohmann::json;

class Shape : public TypedElement {
public:
    vec3f position;

    Shape(const std::string &id, const std::string &type, const vec3f &position) : TypedElement(id, type), position(position) {}
    Shape() {}
    ~Shape() {}

    virtual bool ray_intersect(const vec3f &, const vec3f &, float &) const { return false; };
};

void from_json(const json &j, Shape &s);
void from_json(const json &j, std::shared_ptr<Shape> &s);
