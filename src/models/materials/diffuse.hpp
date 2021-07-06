#pragma once
#include <iostream>
#include "material.hpp"
#include "utils/vec.hpp"

using json = nlohmann::json;

class Renderer;

struct DiffuseMaterial : Material {
    vec3f rho;

    DiffuseMaterial(const std::string &id, const vec3f &rho) : Material(id, "diffuse"), rho(rho) {}
    
    DiffuseMaterial() {}

    vec3f renderer_get_colour(Renderer &renderer);
};

void from_json(const json &j, DiffuseMaterial &d);
void from_json(const json &j, std::shared_ptr<DiffuseMaterial> &d);
