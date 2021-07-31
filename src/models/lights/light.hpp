#pragma once
#include <iostream>

#include "models/rays/ray.hpp"
#include "utils/vec.hpp"
#include "utils/mat.hpp"

enum LightType { LIGHT_NONE = -1, LIGHT_POINT, LIGHT_AREA };

NLOHMANN_JSON_SERIALIZE_ENUM(LightType, {
    { LIGHT_NONE, nullptr},
    { LIGHT_POINT, "point" },
    { LIGHT_AREA, "area" }
})

struct Light {
    std::string id;
    LightType type;

    vec3f intensity;

    Light(const std::string &id, const LightType type, const vec3f &intensity)
        :
            id(id),
            type(type),
            intensity(intensity) {}

    Light() {}
    virtual void illuminate(const RayInfo &, const vec3f &, const vec3f &, vec3f &, vec3f &, float &) const {};
};

void from_json(const json &j, Light &l);
void from_json(const json &j, std::shared_ptr<Light> &l);
