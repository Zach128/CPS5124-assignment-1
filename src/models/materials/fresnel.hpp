#pragma once
#include <iostream>
#include "material.hpp"
#include "utils/vec.hpp"

using json = nlohmann::json;

class Renderer;

struct FresnelMaterial : Material {
    vec3f rhoR;
    vec3f rhoT;
    float eta;

    FresnelMaterial(const std::string &id, const vec3f &rhoR, const vec3f &rhoT, const float eta) : Material(id, "fresnel dielectric"), rhoR(rhoR), rhoT(rhoT), eta(eta) {}

    FresnelMaterial() {}

    vec3f get_diffuse(Renderer &renderer);
    vec3f get_specular(Renderer &renderer);
    vec3f get_transmission();
    float get_eta();
};

void from_json(const json &j, FresnelMaterial &d);
void from_json(const json &j, std::shared_ptr<FresnelMaterial> &d);
