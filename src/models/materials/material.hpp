#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include "models/object.hpp"

using json = nlohmann::json;

struct Material : TypedElement {
    Material(const std::string &id, const std::string &type) : TypedElement(id, type) {}

    Material() {}
};

void from_json(const json &j, Material &m);
void from_json(const json &j, std::shared_ptr<Material> &m);
