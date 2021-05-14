#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include "utils/vec.hpp"
#include "models/cameras/camera.hpp"
#include "models/cameras/pinhole.hpp"

using json = nlohmann::json;

Camera LoadCamera(const json &j) {
    if (j["type"] == "pinhole") {
        return PinholeCamera(
            j["id"].get<std::string>(),
            j["fov"].get<float>(),
            j["aspect"].get<float>(),
            j["distance"].get<float>(),
            j["position"].get<std::vector<float>>().data(),
            j["target"].get<std::vector<float>>().data()
        );
    } else {
        throw std::runtime_error("Unrecognised camera \"" + j["type"].get<std::string>() + "\" in scene file.\n");
    }
}

void LoadCameras(const json &j, std::vector<Camera> &cameras) {
    cameras = std::vector<Camera>();

    if (!j["cameras"].empty()) {
        for (json raw_cam : j["cameras"]) {
            cameras.push_back(LoadCamera(raw_cam));
        }
    }
}
