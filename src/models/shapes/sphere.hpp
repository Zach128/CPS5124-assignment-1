#pragma once
#include <iostream>
#include "utils/vec.hpp"
#include "models/shapes/shape.hpp"

using json = nlohmann::json;

class Sphere : public Shape {
public:
    float radius;

    Sphere(const std::string &id, const std::string &type, const vec3f &position, const float radius) : Shape(id, type, position), radius(radius) {}
    Sphere() : Shape() {}
    ~Sphere() {}

    bool ray_intersect(const vec3f &orig, const vec3f &dir, float &t0) const;
};

void from_json(const json &j, Sphere &s);
void from_json(const json &j, std::shared_ptr<Sphere> &s);
