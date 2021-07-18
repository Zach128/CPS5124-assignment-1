#pragma once
#include <iostream>

#include "models/rays/ray.hpp"
#include "models/object.hpp"
#include "utils/vec.hpp"
#include "utils/mat.hpp"

struct Light : TypedElement {
    vec3f intensity;
    Matrix44f lightToWorld;

    Light(const std::string &id, const std::string &type, const vec3f &intensity)
        : TypedElement(id, type),
          intensity(intensity) {}

    Light() {}
    virtual void illuminate(const RayInfo &, const vec3f &, const vec3f &, vec3f &, vec3f &, float &) const {};
};

void from_json(const json &j, Light &l);
void from_json(const json &j, std::shared_ptr<Light> &l);
