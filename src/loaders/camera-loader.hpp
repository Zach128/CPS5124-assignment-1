#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include "utils/vec.hpp"
#include "models/cameras/camera.hpp"
#include "models/cameras/pinhole.hpp"

using json = nlohmann::json;

Camera LoadCamera(const json &j) {
    Camera c;

    if (j["type"] == "pinhole") {
        c = j.get<PinholeCamera>();
    } else {
        c = j.get<Camera>();
    }

    return c;
}

void LoadCameras(const json &j, std::vector<Camera> &cameras) {
    cameras = std::vector<Camera>();

    if (!j["cameras"].empty()) {
        for (json raw_cam : j["cameras"]) {
            cameras.push_back(LoadCamera(raw_cam));
        }
    }
}

void LoadCameras(const json &j, std::vector<std::shared_ptr<Camera>> &cameras) {
    cameras = std::vector<std::shared_ptr<Camera>>();

    if (!j.at("cameras").empty()) {
        for (json cam : j.at("cameras")) {
            if (cam.at("type") == "pinhole") {
                cameras.push_back(cam.get<std::shared_ptr<PinholeCamera>>());
            } else {
                cameras.push_back(cam.get<std::shared_ptr<Camera>>());
            }
        }
    }
}
