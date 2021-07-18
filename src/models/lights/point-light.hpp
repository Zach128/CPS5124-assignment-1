#pragma once
#include <iostream>
#include "models/object.hpp"
#include "utils/vec.hpp"
#include "models/lights/light.hpp"

struct PointLight : Light {
    vec3f position;

    PointLight(const std::string &id, const std::string &type, const vec3f &position, const vec3f &intensity)
        : Light(id, type, intensity), position(position) {}

    PointLight() {}

    void illuminate(const RayInfo &srcRay, const vec3f &hit, const vec3f &N, vec3f &lightDir, vec3f &lightIntensity, float &distance) const;
};

void from_json(const json &j, PointLight &l);
void from_json(const json &j, std::shared_ptr<PointLight> &l);
