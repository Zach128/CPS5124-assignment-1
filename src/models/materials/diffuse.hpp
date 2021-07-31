#pragma once
#include <iostream>
#include "material.hpp"
#include "utils/vec.hpp"

using json = nlohmann::json;

class Renderer;

struct DiffuseMaterial : Material {
    vec3f rho;

    DiffuseMaterial(const std::string &id, const vec3f &rho) : Material(id, MaterialType::MATERIAL_DIFFUSE), rho(rho) {}

    DiffuseMaterial() {}

    vec3f get_diffuse();
};

void from_json(const json &j, DiffuseMaterial &d);
void from_json(const json &j, std::shared_ptr<DiffuseMaterial> &d);
