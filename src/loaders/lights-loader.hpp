#pragma once
#include <iostream>
#include <algorithm>
#include <nlohmann/json.hpp>

#include "models/lights/light.hpp"
#include "models/lights/area-light.hpp"
#include "models/lights/point-light.hpp"

using json = nlohmann::json;

void LoadLights(const json &j, std::vector<std::shared_ptr<Light>> &lights, const std::vector<std::string> &activeLightIds) {
    lights = std::vector<std::shared_ptr<Light>>();

    if (!j.at("lights").empty()) {
        for (json light : j.at("lights")) {
            auto found_light = std::find(activeLightIds.begin(), activeLightIds.end(), light.at("id").get<std::string>());

            // Filter the loaded lights by the list of ids provided.
            if (found_light != activeLightIds.end()) {
                if (light.at("type") == "point") {
                    lights.push_back(light.get<std::shared_ptr<PointLight>>());
                } else if (light.at("type") == "area") {
                    lights.push_back(light.get<std::shared_ptr<AreaLight>>());
                } else {
                    lights.push_back(light.get<std::shared_ptr<Light>>());
                }
            }
        }
    }
}
