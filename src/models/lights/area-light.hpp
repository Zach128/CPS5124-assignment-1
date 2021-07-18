#pragma once
#include <iostream>
#include <string>
#include "models/object.hpp"
#include "models/shapes/shape.hpp"
#include "utils/vec.hpp"
#include "models/lights/light.hpp"

struct AreaLight : Light {
    std::string shape_id;
    std::shared_ptr<Shape> shape;

    AreaLight(const std::string &id, const std::string &type, const std::string &shape_id, const vec3f &intensity)
        : Light(id, type, intensity), shape_id(shape_id) {}

    AreaLight() {}

    void illuminate(const RayInfo &srcRay, const vec3f &hit, const vec3f &N, vec3f &lightDir, vec3f &lightIntensity, float &distance) const;
};

void from_json(const json &j, AreaLight &l);
void from_json(const json &j, std::shared_ptr<AreaLight> &l);
