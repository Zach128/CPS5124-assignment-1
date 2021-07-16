#pragma once
#include <iostream>
#include "models/object.hpp"
#include "utils/vec.hpp"
#include "models/lights/light.hpp"

struct PointLight : Light {
    PointLight(const std::string &id, const std::string &type, const vec3f &position, const vec3f &intensity)
        : Light(id, type, position, intensity) {}

    PointLight() {}

    void illuminate(const vec3f &P, vec3f &lightDir, vec3f &lightIntensity, float &distance) const;
};

void from_json(const json &j, PointLight &l);
void from_json(const json &j, std::shared_ptr<PointLight> &l);
