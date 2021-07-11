#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

#include "utils/vec.hpp"
#include "models/object.hpp"

using json = nlohmann::json;

class Renderer;

struct Material : TypedElement {
    Material(const std::string &id, const std::string &type) : TypedElement(id, type) {}

    Material() {}

    virtual vec3f get_diffuse(Renderer &) { return vec3f(0, 0, 0); };
};

void from_json(const json &j, Material &m);
void from_json(const json &j, std::shared_ptr<Material> &m);
