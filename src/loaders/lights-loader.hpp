#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include "models/lights/light.hpp"

using json = nlohmann::json;

void LoadLights(const json &j, std::vector<std::shared_ptr<Light>> &lights) {
    lights = std::vector<std::shared_ptr<Light>>();

    if (!j.at("lights").empty()) {
        for (json light : j.at("lights")) {
            lights.push_back(light.get<std::shared_ptr<Light>>());
        }
    }
}
