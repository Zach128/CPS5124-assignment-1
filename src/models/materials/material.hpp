#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

#include "utils/vec.hpp"

using json = nlohmann::json;

class Renderer;

enum MaterialType { MATERIAL_NONE = -1, MATERIAL_DIFFUSE, MATERIAL_SPECULAR, MATERIAL_GLOSSY, MATERIAL_FRESNEL };

NLOHMANN_JSON_SERIALIZE_ENUM(MaterialType, {
    { MATERIAL_NONE, nullptr},
    { MATERIAL_DIFFUSE, "diffuse" },
    { MATERIAL_SPECULAR, "specular reflection" },
    { MATERIAL_GLOSSY, "glossy reflection" },
    { MATERIAL_FRESNEL, "fresnel dielectric" }
})

struct Material {
    std::string id;
    MaterialType type;

    Material(const std::string &id, const MaterialType type) : id(id), type(type) {}

    Material() {}

    virtual vec3f get_diffuse() { return vec3f(0, 0, 0); };
    virtual vec3f get_specular() { return vec3f(0, 0, 0); };
    virtual float get_roughness() { return 1; };
    virtual vec3f get_transmission() { return vec3f(0, 0, 0); };
    virtual float get_eta() { return 1; };
};

void from_json(const json &j, Material &m);
void from_json(const json &j, std::shared_ptr<Material> &m);
