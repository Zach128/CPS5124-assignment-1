#pragma once
#include <iostream>
#include "material.hpp"
#include "utils/vec.hpp"

using json = nlohmann::json;

struct DiffuseMaterial : Material {
    vec3f rho;

    DiffuseMaterial(const std::string &id, const vec3f &rho) : Material(id, "diffuse"), rho(rho) {}
    
    DiffuseMaterial() {}
};

void from_json(const json &j, DiffuseMaterial &d) {
    nlohmann::from_json(j, static_cast<Material &>(d));

    d.rho = vec3f(j.at("rho").get<std::vector<float>>().data());
}

void from_json(const json &j, std::shared_ptr<DiffuseMaterial> &d) {
    d = std::make_shared<DiffuseMaterial>();
    
    j.at("id").get_to(d->id);
    j.at("type").get_to(d->type);

    d->rho = vec3f(j.at("rho").get<std::vector<float>>().data());
}
