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
