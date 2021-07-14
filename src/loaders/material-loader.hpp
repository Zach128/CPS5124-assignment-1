#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include "utils/vec.hpp"
#include "models/materials/material.hpp"
#include "models/materials/diffuse.hpp"
#include "models/materials/specular.hpp"
#include "models/materials/glossy.hpp"
#include "models/materials/fresnel.hpp"

using json = nlohmann::json;

void LoadMaterials(const json &j, std::vector<std::shared_ptr<Material>> &materials) {
    materials = std::vector<std::shared_ptr<Material>>();

    if (!j.at("materials").empty()) {
        for (json mat : j.at("materials")) {
            if (mat.at("type") == "diffuse") {
                materials.push_back(mat.get<std::shared_ptr<DiffuseMaterial>>());
            } else if (mat.at("type") == "specular reflection") {
                materials.push_back(mat.get<std::shared_ptr<SpecularMaterial>>());
            } else if (mat.at("type") == "glossy reflection") {
                materials.push_back(mat.get<std::shared_ptr<GlossyMaterial>>());
            } else if (mat.at("type") == "fresnel dielectric") {
                materials.push_back(mat.get<std::shared_ptr<FresnelMaterial>>());
            } else {
                materials.push_back(mat.get<std::shared_ptr<Material>>());
            }
        }
    }
}
