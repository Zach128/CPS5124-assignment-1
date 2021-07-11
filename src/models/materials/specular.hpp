#pragma once
#include <iostream>
#include "material.hpp"
#include "utils/vec.hpp"

using json = nlohmann::json;

class Renderer;

struct SpecularMaterial : Material {
    vec3f rho;

    SpecularMaterial(const std::string &id, const vec3f &rho) : Material(id, "specular reflection"), rho(rho) {}

    SpecularMaterial() {}

    vec3f get_diffuse(Renderer &renderer);
    vec3f get_specular(Renderer &renderer);
};

void from_json(const json &j, SpecularMaterial &d);
void from_json(const json &j, std::shared_ptr<SpecularMaterial> &d);
