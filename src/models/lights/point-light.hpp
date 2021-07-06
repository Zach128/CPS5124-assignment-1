#pragma once
#include <iostream>
#include "models/object.hpp"
#include "utils/vec.hpp"
#include "models/lights/light.hpp"

struct PointLight : Light {
    PointLight(const std::string &id, const std::string &type, const vec3f &position, const vec3f &intensity)
        : Light(id, type, position, intensity) {}
    
    PointLight() {}
};

void from_json(const json &j, PointLight &l);
void from_json(const json &j, std::shared_ptr<PointLight> &l);
