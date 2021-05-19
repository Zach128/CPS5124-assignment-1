#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include "utils/vec.hpp"
#include "models/shapes/shape.hpp"
#include "models/shapes/sphere.hpp"

using json = nlohmann::json;

void LoadShapes(const json &j, std::vector<std::shared_ptr<Shape>> &shapes) {
    shapes = std::vector<std::shared_ptr<Shape>>();

    if (!j.at("shapes").empty()) {
        for (json shape : j.at("shapes")) {
            if (shape.at("type") == "sphere") {
                shapes.push_back(shape.get<std::shared_ptr<Sphere>>());
            } else {
                shapes.push_back(shape.get<std::shared_ptr<Shape>>());
            }
        }
    }
}
