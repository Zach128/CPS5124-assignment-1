#pragma once
#include <iostream>
#include "utils/vec.hpp"
#include "models/shapes/shape.hpp"
#include "models/rays/ray.hpp"

using json = nlohmann::json;

// Forward declare visitor class
class Renderer;

class Sphere : public Shape {
public:
    float radius;

    Sphere(const std::string &id, const std::string &type, const vec3f &position, const float radius) : Shape(id, type, position), radius(radius) {}
    Sphere() : Shape() {}
    ~Sphere() {}

    bool ray_intersect(const RayInfo &ray, float &t0) const;
    void getSurfaceProperties( const vec3f &hitPoint, vec3f &hitNormal) const {
        hitNormal = hitPoint - position;
        hitNormal.normalize();
    }
};

void from_json(const json &j, Sphere &s);
void from_json(const json &j, std::shared_ptr<Sphere> &s);
