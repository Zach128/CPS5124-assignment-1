#pragma once
#include <iostream>
#include "utils/vec.hpp"
#include "models/object.hpp"

using json = nlohmann::json;

// Forward declare visitor class
class Renderer;

class Shape : public TypedElement {
public:
    vec3f position;

    Shape(const std::string &id, const std::string &type, const vec3f &position) : TypedElement(id, type), position(position) {}
    Shape() {}
    ~Shape() {}

    virtual bool renderer_ray_intersect(const Renderer &, float &t0) const { t0 = 0; return false; }
};

void from_json(const json &j, Shape &s);
void from_json(const json &j, std::shared_ptr<Shape> &s);
