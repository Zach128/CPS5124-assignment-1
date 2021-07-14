#pragma once
#include <iostream>
#include "material.hpp"
#include "utils/vec.hpp"

using json = nlohmann::json;

class Renderer;

struct GlossyMaterial : Material {
    vec3f rho;
    float roughness;

    GlossyMaterial(const std::string &id, const vec3f &rho, const float roughness) : Material(id, "glossy reflection"), rho(rho), roughness(roughness) {}

    GlossyMaterial() {}

    vec3f get_diffuse();
    vec3f get_specular();
    float get_roughness();
};

void from_json(const json &j, GlossyMaterial &d);
void from_json(const json &j, std::shared_ptr<GlossyMaterial> &d);
