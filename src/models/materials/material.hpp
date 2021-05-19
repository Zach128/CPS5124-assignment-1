#pragma once
#include <iostream>
#include "models/object.hpp"

using json = nlohmann::json;

struct Material : TypedElement {
    Material(const std::string &id, const std::string &type) : TypedElement(id, type) {}

    Material() {}
};

void from_json(const json &j, Material &m) {
  j.at("id").get_to(m.id);
  j.at("type").get_to(m.type);
}

void from_json(const json &j, std::shared_ptr<Material> &m) {
  m = std::make_shared<Material>();
  
  j.at("id").get_to(m->id);
  j.at("type").get_to(m->type);
}